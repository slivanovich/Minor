#ifndef SERVER_HPP
#define SERVER_HPP

#include "../def.hpp"
#include "../request_handler/request_handler.hpp"

namespace Minor {
class ServerHandler {
    int port;
    Server server;
    std::unordered_map<std::string, std::vector<Server::connection_ptr>> connections;
    std::vector<std::thread> threads;

    std::unique_ptr<RequestHandler> requestHandler;

    static void onMessage(ServerHandler *serverHandler, Server::connection_ptr &connection, const Server::message_ptr &message);
    static void onOpenConnection(ServerHandler *serverHandler, const Server::connection_ptr &connection);
    static void onCloseConnection(ServerHandler *serverHandler, const Server::connection_ptr &connection);

    void splitRequest(const std::string &request, std::vector<std::string> &data);

public:
    explicit ServerHandler();
    void init(int port_);
    void stop(const std::string &username, const std::string &token);
    ~ServerHandler();
};
}  // namespace Minor

#endif  // SERVER_HPP