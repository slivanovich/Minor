cmake_minimum_required(VERSION 3.21)
project(minor_server)

set(CMAKE_CXX_STANDARD 17)

if (APPLE)
    message(STATUS ">>> Apple detected")
    include_directories(include)
    include_directories(../include)
    # link_directories(${CMAKE_SOURCE_DIR}/lib)
    # file(GLOB LIBRARIES "${CMAKE_SOURCE_DIR}/lib/macos/*.dylib")
elseif (UNIX AND NOT APPLE)
    message(STATUS ">>> Ubuntu detected")
    include_directories(include)
    include_directories(../include)
    set(LIBRARIES pqxx pq)
else ()
    message(FATAL_ERROR "Not support this system")
endif ()

file(GLOB request_handler_source "source/request_handler/*.cpp")
file(GLOB user_source "source/user/*.cpp")
file(GLOB accounting_source "source/request_handler/accounting/*.cpp")
file(GLOB accounting_database_source "source/request_handler/accounting/database/*.cpp")
file(GLOB server_source "source/server/*.cpp")

add_executable(minor_server
    source/main.cpp
    ${request_handler_source}

    ${user_source}

    ${accounting_source}
    ${accounting_database_source}

    ${server_source}
)
target_link_libraries(minor_server ${LIBRARIES})