
#include "targetver.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <thread>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

void handle_request(http::request<http::string_body> req, std::shared_ptr<tcp::socket> socket) {
    // Create a response
    http::response<http::string_body> res{ http::status::ok, req.version() };
    res.set(http::field::server, "Boost.Beast");
    res.set(http::field::content_type, "text/plain");
    res.body() = "Hello, Kubernetes from Visual Studio!";
    res.prepare_payload();

    // Write the response
    http::write(*socket, res);
}

void do_session(std::shared_ptr<tcp::socket> socket) {
    try {
        beast::flat_buffer buffer;
        http::request<http::string_body> req;

        // Read the request
        http::read(*socket, buffer, req);

        // Handle the request
        handle_request(std::move(req), socket);

        // Close the socket
        beast::error_code ec;
        socket->shutdown(tcp::socket::shutdown_send, ec);
    }
    catch (const std::exception& e) {
        std::cerr << "Session error: " << e.what() << std::endl;
    }
}

void do_accept(tcp::acceptor& acceptor, net::io_context& ioc) {
    acceptor.async_accept([&](beast::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::make_shared<std::thread>([s = std::make_shared<tcp::socket>(std::move(socket))]() {
                do_session(s);
                })->detach();
        }
        do_accept(acceptor, ioc);
        });
}

int main() {
    try {
        net::io_context ioc{ 1 };
        tcp::acceptor acceptor{ ioc, {tcp::v4(), 8080} };

        do_accept(acceptor, ioc);

        std::cout << "Server is listening on port 8080...\n";
        ioc.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
