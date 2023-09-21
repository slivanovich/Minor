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

#endif // DEF_HPP