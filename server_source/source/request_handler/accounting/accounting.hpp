#ifndef GRIDERY_ACCOUNTING_HPP
#define GRIDERY_ACCOUNTING_HPP

#include "../../def.hpp"
#include "../user/user.hpp"
#include "database/database.hpp"

namespace Minor {
class AccountingHandler {
    std::mutex accountingMutex;
    std::unordered_map<std::string, std::string> &usernameByToken;
    std::unordered_map<std::string, std::unique_ptr<User>> &users;

    std::unique_ptr<AccountingDatabase> accountingDatabase;

public:
    struct Response {
        bool isContinue;
        std::string username;
        std::string token;
        Specifier specifier;
    };

private:
    static std::string generateToken();

    Response registerUser(const std::vector<std::string> &data);
    Response loginUser(const std::vector<std::string> &data);
    Response logoutUser(const std::string &username, const std::string &token = "");
    Response changeUsername(const std::string &token, const std::vector<std::string> &data);
    Response changePassword(const std::string &token, const std::vector<std::string> &data);
    Response deleteUserItself(const std::string &username, const std::string &token);
    Response deleteUserByName(const std::string &username, const std::string &otherUsername);

public:
    explicit AccountingHandler(
        std::unordered_map<std::string, std::string> &usernameByToken_,
        std::unordered_map<std::string, std::unique_ptr<User>> &users_
    );

    Response onRequest(const std::vector<std::string> &data);
};
}

#endif  // GRIDERY_ACCOUNTING_HPP
