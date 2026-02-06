cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "all")

project(TCP_cpp)

add_executable(server server.cpp)
add_executable(client client.cpp)

