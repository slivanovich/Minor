#ifndef GRIDERY_ACCOUNTING_DATABASE_HPP
#define GRIDERY_ACCOUNTING_DATABASE_HPP

#include <pqxx/pqxx>

#include "../../../def.hpp"

namespace Minor {
class AccountingDatabase {
#define LOG(tag, message)                                \
    {                                                    \
        if (showLogs) output::console({{tag, message}}); \
    }

    const bool showLogs = true;
    const bool showRequestLogs = true;
    std::unique_ptr<pqxx::connection> connection;

    pqxx::result smartQuery(pqxx::work &query, const std::string &request) const;

public:
    explicit AccountingDatabase();

    [[nodiscard]] Specifier loginUser(const std::string &username, const std::string &password) const;
    [[nodiscard]] Specifier registerUser(
        const std::string &username,
        const std::string &password
    ) const;

    [[nodiscard]] Specifier changeUsername(const std::string &oldUsername, const std::string &newUsername);
    [[nodiscard]] Specifier changePassword(const std::string &username, const std::string &newPassword);

    [[nodiscard]] Specifier deleteUser(const std::string &username) const;
};
}

#endif  // GRIDERY_ACCOUNTING_DATABASE_HPP
