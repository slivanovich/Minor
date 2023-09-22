#ifndef DEF_HPP
#define DEF_HPP

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <exception>
#include <signal.h>

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include "utilities.hpp"

typedef websocketpp::server<websocketpp::config::asio> Server;

enum Specifier {
    OK,
    NONE,
    MAKEDIR,
    LOGIN_USER,
    LOGOUT_USER,
    DELETE_USER,
    DISCONNECT_USER,
    UNKNOWN_REQUEST,
    USER_EXIST,
    USER_NOT_EXIST,
    FILE_EXISTS,
    FILE_NOT_EXIST,
    TAG_EXISTS,
    TAG_NOT_EXIST,
    WRONG_PASSWORD,
    PASSWORD_TOO_LONG,
    PASSWORD_TOO_SHORT,
    USERNAME_TOO_LONG,
    TAG_CHILD_NOT_FOUND,
    TAG_PARENT_NOT_EXIST,
};

#endif // DEF_HPP