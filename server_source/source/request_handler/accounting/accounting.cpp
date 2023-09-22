#include "accounting.hpp"

namespace Minor {
std::string AccountingHandler::generateToken() {
    std::string token;
    token.resize(20);
    for (auto &sign : token) {
        // NOLINTNEXTLINE
        char newSign = static_cast<char>(static_cast<int>(rand() / static_cast<float>(RAND_MAX) * ('z' - '0') + '0'));
        sign = newSign;
    }
    return token;
}

AccountingHandler::Response AccountingHandler::registerUser(const std::vector<std::string> &data) {
    const std::string &username = data[0];
    const std::string &password = data[1];
    switch (accountingDatabase->registerUser(username, password)) {
        case USER_EXIST:
            return {false, username, "", Specifier::USER_EXIST};
        case USERNAME_TOO_LONG:
            return {false, username, "", Specifier::USERNAME_TOO_LONG};
        case PASSWORD_TOO_SHORT:
            return {false, username, "", Specifier::PASSWORD_TOO_SHORT};
        case PASSWORD_TOO_LONG:
            return {false, username, "", Specifier::PASSWORD_TOO_LONG};
        case OK: {
            users[username] = std::make_unique<User>(username, password);
            User *user = users[username].get();
            const std::string token = generateToken();
            usernameByToken[token] = username;
            output::console(
                {{"REGISTER", ""}, {"USERNAME", username}, {"PASSWORD", password}, {"TOKEN", token}, {"PERMISSION", "guest"}}
            );
            return {false, username, token, Specifier::LOGIN_USER};
        }
        default:
            return {false, "", "", Specifier::NONE};
            break;
    }
}

AccountingHandler::Response AccountingHandler::loginUser(const std::vector<std::string> &data) {
    const std::string &username = data[0];
    const std::string &password = data[1];
    switch (accountingDatabase->loginUser(username, password)) {
        case USER_NOT_EXIST:
            return {false, username, "", Specifier::USER_NOT_EXIST};
        case WRONG_PASSWORD:
            return {false, username, "", Specifier::WRONG_PASSWORD};
        case OK: {
            users[username] = std::make_unique<User>(username, password);
            User *user = users[username].get();
            const std::string token = generateToken();
            usernameByToken[token] = username;
            output::console(
                {{"LOGIN", ""}, {"USERNAME", username}, {"PASSWORD", password}, {"TOKEN", token}}
            );
            return {false, username, token, Specifier::LOGIN_USER};
        }
        default:
            return {false, "", "", Specifier::NONE};
    }
}

AccountingHandler::Response AccountingHandler::logoutUser(const std::string &username, const std::string &token) {
    if (!token.empty()) usernameByToken.erase(token);
    output::console({{"LOGOUT", ""}, {"NAME", username}, {"TOKEN", token}, {"IP", "none"}});
    users.erase(username);
    return {false, username, token, Specifier::LOGOUT_USER};
}

AccountingHandler::Response AccountingHandler::changeUsername(
    const std::string &token,
    const std::vector<std::string> &data
) {
    const std::string &password = data[0];
    const std::string &newUsername = data[1];

    std::unique_ptr<User> user = std::exchange(users[usernameByToken[token]], nullptr);
    if (user != nullptr) {
        user->setName(newUsername);
        usernameByToken[token] = newUsername;
        users[newUsername] = std::exchange(user, nullptr);
        // accountingDatabase->changeUsername();
    }

    output::console(
        {{"CHANGE USERNAME", ""}, {"NAME", usernameByToken[token]}, {"NEW NAME", newUsername}, {"TOKEN", token}, {"IP", "none"}}
    );

    return {false, newUsername, token, Specifier::NONE};
}

// TODO: Email conformation
AccountingHandler::Response AccountingHandler::changePassword(
    const std::string &token,
    const std::vector<std::string> &data
) {
    const std::string &newPassword = data[0];

    User *user = users[usernameByToken[token]].get();
    if (user != nullptr) {
        user->setPassword(newPassword);
        accountingDatabase->changePassword(usernameByToken[token], newPassword);
    }

    output::console(
        {{"CHANGE PASSWORD", ""},
         {"NAME", usernameByToken[token]},
         {"NEW PASSWORD", newPassword},
         {"TOKEN", token},
         {"IP", "none"}}
    );

    return {false, user->getName(), token, Specifier::NONE};
}

AccountingHandler::Response AccountingHandler::deleteUserItself(const std::string &username, const std::string &token) {
    const auto response = accountingDatabase->deleteUser(username);
    switch (response) {
        case OK:
            output::console({{"DELETED", ""}, {"NAME", username}, {"TOKEN", token}, {"IP", "none"}});
            return logoutUser(username, token);
        default:
            return {false, username, "", response};
    }
}

AccountingHandler::Response AccountingHandler::deleteUserByName(const std::string &username, const std::string &otherUsername) {
    if (username.empty()) return {false, "", "", USER_NOT_EXIST};
    const auto response = accountingDatabase->deleteUser(otherUsername);
    switch (response) {
        case OK:
            output::console({{"DELETED", "by " + username}, {"NAME", otherUsername}, {"IP", "none"}});
            return logoutUser(otherUsername);
        default:
            return {false, username, "", response};
    }
}

AccountingHandler::AccountingHandler(
    std::unordered_map<std::string, std::string> &usernameByToken_,
    std::unordered_map<std::string, std::unique_ptr<User>> &users_
)
    : usernameByToken(usernameByToken_), users(users_) {
    accountingDatabase = std::make_unique<AccountingDatabase>();
}

AccountingHandler::Response AccountingHandler::onRequest(const std::vector<std::string> &data) {
    std::string command = data[0];
    std::vector<std::string> tmp_data;
    for (size_t index = 1; index < data.size(); index++)
        tmp.push_back(data[index]);

    if (command == "register") {
        return registerUser(tmp_data);
    } else if (command == "login") {
        return loginUser(tmp_data);
    } else {
        const std::string &username = usernameByToken[token];
        if (username.empty())
            return {false, "", "", Specifier::UNKNOWN_REQUEST};
        else {
            if (command == "logout") {
                return logoutUser(username, token);
            } else if (command == "delete user") {
                return deleteUserByName(username, tmp_data[0]);
            } else if (command == "delete me") {
                return deleteUserItself(username, token);
            } else if (command == "disconnect") {
                auto response = logoutUser(username, token);
                switch (response.specifier) {
                    case LOGOUT_USER:
                        return {false, username, token, DISCONNECT_USER};
                    default:
                        return response;
                }
            } else if (command == "change password") {
                return changePassword(token, tmp_data);
            } else if (command == "change username") {
                return changeUsername(token, tmp_data);
            } else {
                return {true, "", "", UNKNOWN_REQUEST};
            }
        }
    }
    return {true, "", "", UNKNOWN_REQUEST};
}
}
