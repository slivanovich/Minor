#include "def.hpp"
#include "server/server.hpp"

Minor::ServerHandler serverHandler;

int main() {
    std::signal(SIGINT, [](int signum) {
        serverHandler.stop("", "");
    });
    serverHandler.init(8080);
    return 0;
}