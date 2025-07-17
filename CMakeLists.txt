cmake_minimum_required(VERSION 3.16)
project(socket_programing)

set(CMAKE_CXX_STANDARD 20)

add_executable(socket_programing server.cpp
        server.cpp)
add_executable(client client.cpp
        client.cpp)