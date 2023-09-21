#include <iostream>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

typedef websocketpp::client<websocketpp::config::asio_client> Client;

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

void on_open(Client *client, websocketpp::connection_hdl hdl) {
    std::string message = "Hello";
    client->send(hdl, message, websocketpp::frame::opcode::text);
    std::cout << "Send message: " << message << std::endl;
}

void on_fail(Client *client, websocketpp::connection_hdl hdl) {
    std::cout << "Connection failed" << std::endl;
}

void on_message(Client *client, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "Received Reply: " << msg->get_payload() << std::endl;
    client->close(hdl, websocketpp::close::status::normal, "Client end");
}

void on_close(Client *client, websocketpp::connection_hdl hdl) {
    std::cout << "Connection closed" << std::endl;
}

int main() {
    Client client;

    const std::string uri = "ws://localhost:8080";

    try {
        client.clear_access_channels(websocketpp::log::alevel::all);

        client.init_asio();

        client.set_open_handler(bind(&on_open, &client, ::_1));
        client.set_fail_handler(bind(&on_fail, &client, ::_1));
        client.set_message_handler(bind(&on_message, &client, ::_1, ::_2));
        client.set_close_handler(bind(&on_close, &client, ::_1));

        websocketpp::lib::error_code ec;
        Client::connection_ptr connection = client.get_connection(uri, ec);
        client.connect(connection);

        client.run();
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    } catch (websocketpp::lib::error_code e) {
        std::cout << e.message() << std::endl;
    }
}
