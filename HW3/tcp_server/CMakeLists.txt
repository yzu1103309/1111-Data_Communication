cmake_minimum_required(VERSION 3.24)
project(tcp_server)

set(CMAKE_CXX_STANDARD 14)

add_executable(tcp_server
        server.cpp)
