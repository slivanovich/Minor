#include "server.hpp"

namespace Minor {
ServerHandler::ServerHandler() {
}

void ServerHandler::init(int port_) {
    port = port_;
    try {
        server.clear_access_channels(
            websocketpp::log::alevel::frame_header | websocketpp::log::alevel::frame_payload | websocketpp::log::alevel::connect |
            websocketpp::log::alevel::disconnect | websocketpp::log::alevel::control
        );
        server.set_reuse_addr(true);
        server.init_asio();

        server.set_open_handler([&](const websocketpp::connection_hdl &hdl) -> void {
            Server::connection_ptr connection = server.get_con_from_hdl(hdl);
            return onOpenConnection(this, connection);
        });
        server.set_close_handler([&](const websocketpp::connection_hdl &hdl) -> void {
            Server::connection_ptr connection = server.get_con_from_hdl(hdl);
            return onCloseConnection(this, connection);
        });
        server.set_message_handler([&](const websocketpp::connection_hdl &hdl, const Server::message_ptr &message) -> void {
            Server::connection_ptr connection = server.get_con_from_hdl(hdl);
            return onMessage(this, connection, message);
        });

        output::console({{"SERVER", "STARTED"}});

        server.listen(port);
        server.start_accept();
        server.run();
    } catch (websocketpp::exception &exception) {
        EXCEPTION(exception.what());
    } catch (std::exception &exception) {
        EXCEPTION(exception.what());
    }
}

void ServerHandler::stop(const std::string &username, const std::string &token) {
    if (!username.empty()) {
        output::console({{"MESSAGE", ""}, {"USERNAME", username}, {"ACCESS", "by token"}, {"REQUEST", "stop"}});
    } else
        output::console({{"STOPPED", "by server itself"}});
    try {
        // for (auto &thread : threads)
        //     thread.join();
        // for (auto &[key, connectionsVector] : connections)
        //     for (auto &connection : connectionsVector)
        //         connection->close(0, "Server destructor");
        server.stop();
    } catch (const websocketpp::exception &exception) {
        EXCEPTION(exception.what());
    }
}

ServerHandler::~ServerHandler() {
    output::console({{"EXECUTING", "SERVER HANDLER DESTRUCTOR"}});
    // stop("", "none");
}
}  // namespace Minor