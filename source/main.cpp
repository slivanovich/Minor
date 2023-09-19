#include "def.hpp"
#include "server/server.hpp"

int main() {
    Minor::ServerHandler serverHandler(8080);
    return 0;
}