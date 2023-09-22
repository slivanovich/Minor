#ifndef GRIDERY_USER_HPP
#define GRIDERY_USER_HPP

#include "../../def.hpp"

namespace Minor {
class User {
    std::string name;
    std::string password;
    
public:
    enum ResponseType { TEXT, BINARY };

    [[nodiscard]] const std::string &getName() const;
    [[nodiscard]] const std::string &getPassword() const;

    void setName(std::string name_);
    void setPassword(std::string password_);

    explicit User(std::string name_, std::string password_);
    ~User();
};
}

#endif  // GRIDERY_USER_HPP
