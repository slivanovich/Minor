#include "user.hpp"

namespace Minor {
[[nodiscard]] const std::string &User::getName() const {
    return name;
}

[[nodiscard]] const std::string &User::getPassword() const {
    return password;
}

void User::setName(std::string name_) {
    name = std::exchange(name_, "");
}

void User::setPassword(std::string password_) {
    password = std::exchange(password_, "");
}

User::User(std::string name_, std::string password_)
    : name(std::exchange(name_, "")), password(std::exchange(password_, "")) {
}

User::~User() {
    output::console({{"EXECUTING USER DESTRUCTOR", name}});
}
}
