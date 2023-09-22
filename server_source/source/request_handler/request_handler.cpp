#include "request_handler.hpp"

namespace Minor {
RequestHandler::RequestHandler() {
    accountingHandler = std::make_unique<AccountingHandler>(usernameByToken, users);
}

std::vector<RequestHandler::Response> RequestHandler::onRequest(
    const std::vector<std::string> &data
) {
    User *user;
    std::vector<Response> response;
    const AccountingHandler::Response accountingResponse = accountingHandler->onRequest(data);

    switch (accountingResponse.specifier) {
        case LOGIN_USER:
            user = users[accountingResponse.username].get();
            token = accountingResponse.token;
            response.emplace_back("login " + token);
            break;
        case USER_EXIST:
            response.emplace_back("User has been already registered");
            break;
        case USER_NOT_EXIST:
            response.emplace_back("User does not exist");
            break;
        case USERNAME_TOO_LONG:
            response.emplace_back("Username is too long");
            break;
        case PASSWORD_TOO_SHORT:
            response.emplace_back("Password is too short");
            break;
        case PASSWORD_TOO_LONG:
            response.emplace_back("Password is too long");
            break;
        case WRONG_PASSWORD:
            response.emplace_back("Wrong password");
            break;
        case UNKNOWN_REQUEST:
            user = users[usernameByToken[token]].get();
            output::console({{"MESSAGE", "Get request from some logged in user..."}});
            break;
        default:
            output::console({{"WARNING", "Some unresolved specifiers from accounting handler..."}});
            break;
    }
    return response;
}
}
