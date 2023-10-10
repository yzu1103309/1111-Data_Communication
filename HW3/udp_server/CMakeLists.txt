cmake_minimum_required(VERSION 3.24)
project(udp_server)

set(CMAKE_CXX_STANDARD 14)

add_executable(udp_server
        server.cpp)
