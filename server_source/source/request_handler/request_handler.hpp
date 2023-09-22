#ifndef GRIDERY_REQUEST_HANDLER_HPP
#define GRIDERY_REQUEST_HANDLER_HPP

#include "../def.hpp"
#include "user/user.hpp"
#include "accounting/accounting.hpp"

namespace Minor {
class RequestHandler {
    std::mutex requestMutex;
    std::unordered_map<std::string, std::string> usernameByToken;
    std::unordered_map<std::string, std::unique_ptr<User>> users;

    std::unique_ptr<AccountingHandler> accountingHandler;

    void createPermissionTagsTree();

public:
    explicit RequestHandler();

    struct Response {
        std::string data;

        explicit Response(std::string data_) : data(std::exchange(data_, "")) {
        }
    };

    std::vector<Response> onRequest(
        const std::vector<std::string> &data
    );
};
}

#endif  // GRIDERY_REQUEST_HANDLER_HPP
