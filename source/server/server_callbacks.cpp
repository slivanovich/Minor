#include "server.hpp"

namespace Minor {
void ServerHandler::onMessage(
    ServerHandler *serverHandler,
    Server::connection_ptr &connection,
    const Server::message_ptr &message
) {
    serverHandler->threads.emplace_back([serverHandler, connection, message]() {
        const std::string &request = message->get_payload();
        connection->send("request");
    });
    serverHandler->threads.back().detach();
}

void ServerHandler::onOpenConnection(ServerHandler *serverHandler, const Server::connection_ptr &connection) {
    output::console({{"CLIENT CONNECTED", ""}, {"USERNAME", "none"}});
}

void ServerHandler::onCloseConnection(ServerHandler *serverHandler, const Server::connection_ptr &connection) {
    output::console({{"CLIENT DISCONNECTED", ""}, {"USERNAME", "none"}});
}
}  // namespace Gridery