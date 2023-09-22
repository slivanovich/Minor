#include "database.hpp"

namespace Minor {
AccountingDatabase::AccountingDatabase() {
    connection =
        std::make_unique<pqxx::connection>("dbname = postgres user = postgres password = root host = postgresql port = 5432");
    if (connection->is_open()) {
        LOG("SUCCESS", "Database's name for accounting: " + std::string(connection->dbname()));
    } else {
        LOG("ERROR", "Cannot established connection");
    }
}

[[nodiscard]] Specifier AccountingDatabase::loginUser(
    const std::string &username,
    const std::string &password
) const {
    pqxx::work query(*connection);
    try {
        pqxx::result result = smartQuery(query, "SELECT * FROM users WHERE username = \'" + username + "\';");
        if (result.empty()) {
            query.commit();
            LOG("WARNING", "Login failed (" + username + "): wrong username or password");
            return USER_NOT_EXIST;
        }
        const std::string encryptedPassword = pqxx::encrypt_password(username, password);
        if (pqxx::to_string(result[0][1]) != encryptedPassword) {
            query.commit();
            LOG("WARNING", "Login failed (" + username + "): wrong username or password");
            return WRONG_PASSWORD;
        }
        query.commit();
        LOG("MESSAGE", "Login successful (" + username + ")");
        return OK;
    } catch (std::exception &exception) {
        query.abort();
        EXCEPTION(exception.what())
    }
}

[[nodiscard]] Specifier AccountingDatabase::registerUser(
    const std::string &username,
    const std::string &password
) const {
    try {
        if (username.size() > 20) {
            return USERNAME_TOO_LONG;
        } else if (password.size() > 30) {
            return PASSWORD_TOO_LONG;
        } else if (password.size() < 3) {
            return PASSWORD_TOO_SHORT;
        }

        pqxx::work query(*connection);
        if (!smartQuery(query, "SELECT * FROM users WHERE username = \'" + username + "\';").empty()) {
            query.commit();
            LOG("WARNING", "Register failed: user " + username + " already exists");
            return USER_EXIST;
        }

        const std::string encryptedPassword = pqxx::encrypt_password(username, password);
        smartQuery(
            query, "INSERT INTO users (username, password) VALUES (\'" + username + "\', \'" + encryptedPassword + "\');"
        );
        query.commit();
    } catch (std::exception &exception) {
        EXCEPTION(exception.what());
    }
    return OK;
}

[[nodiscard]] Specifier AccountingDatabase::changePassword(const std::string &username, const std::string &newPassword) {
    pqxx::work query(*connection);
    try {
        pqxx::result result =
            smartQuery(query, "UPDATE users WHERE username = \'" + username + "\' SET password = \'" + newPassword + "\';");
        if (result.empty()) {
            LOG("WARNING", "Cannot change password for this user - " + username + ": not exist");
            return USER_NOT_EXIST;
        }
        query.commit();
    } catch (std::exception &exception) {
        query.abort();
        EXCEPTION(exception.what());
    }
    return OK;
}

[[nodiscard]] Specifier AccountingDatabase::deleteUser(const std::string &username) const {
    pqxx::work query(*connection);
    try {
        pqxx::result result = smartQuery(query, "DELETE FROM users WHERE username = \'" + username + "\' RETURNING username;");
        if (result.empty()) {
            LOG("WARNING", "Cannot delete user " + username + ": not exist");
            return USER_NOT_EXIST;
        }
        query.commit();
    } catch (std::exception &exception) {
        query.abort();
        EXCEPTION(exception.what());
    }
    return OK;
}
}
