#include "targetver.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <fstream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

std::vector<std::string> items;
std::mutex items_mutex;

std::string load_html(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) {
        return "<h1>Error loading index.html</h1>";
    }

    return std::string((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
}

std::string generate_item_list() {
    std::string list_html;
    std::lock_guard<std::mutex> lock(items_mutex);
    for (const auto& item : items) {
        list_html += "<li>" + item + "</li>";
    }
    return list_html;
}

void handle_request(http::request<http::string_body> req, std::shared_ptr<tcp::socket> socket) {
    http::response<http::string_body> res{ http::status::ok, req.version() };
    res.set(http::field::server, "Boost.Beast");
    res.set(http::field::content_type, "text/html");

    if (req.method() == http::verb::get) {
        std::string html = load_html("index.html");
        html.replace(html.find("<!-- Items will be dynamically inserted here -->"), 48, generate_item_list());
        res.body() = html;
    }
    else if (req.method() == http::verb::post) {
        auto body = req.body();
        auto pos = body.find("item=");
        if (pos != std::string::npos) {
            std::string item = body.substr(pos + 5);
            std::replace(item.begin(), item.end(), '+', ' ');
            std::lock_guard<std::mutex> lock(items_mutex);
            items.push_back(item);
        }
        res.result(http::status::see_other);
        res.set(http::field::location, "/");
    }
    else {
        res.result(http::status::bad_request);
    }

    res.prepare_payload();
    http::write(*socket, res);
}

void do_session(std::shared_ptr<tcp::socket> socket) {
    try {
        beast::flat_buffer buffer;
        http::request<http::string_body> req;
        http::read(*socket, buffer, req);
        handle_request(std::move(req), socket);
        socket->shutdown(tcp::socket::shutdown_send);
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
