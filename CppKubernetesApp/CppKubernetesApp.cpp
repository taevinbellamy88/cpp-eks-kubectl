#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>

int main() {
    try {
        boost::asio::io_context ioc;

        // Create an HTTP server
        boost::asio::ip::tcp::acceptor acceptor(ioc, {boost::asio::ip::tcp::v4(), 8080});
        boost::asio::ip::tcp::socket socket(ioc);

        std::cout << "Server is listening on port 8080...\n";

        acceptor.accept(socket);
        boost::beast::flat_buffer buffer;

        boost::beast::http::request<boost::beast::http::string_body> request;
        boost::beast::http::read(socket, buffer, request);

        boost::beast::http::response<boost::beast::http::string_body> response{
            boost::beast::http::status::ok, request.version()};
        response.set(boost::beast::http::field::server, "Boost.Beast");
        response.set(boost::beast::http::field::content_type, "text/plain");
        response.body() = "Hello, Kubernetes from Visual Studio!";
        response.prepare_payload();

        boost::beast::http::write(socket, response);
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
