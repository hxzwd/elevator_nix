cmake_minimum_required(VERSION 2.8)
project(elevator)

if(UNIX)
    set(CMAKE_CXX_COMPILER "g++-4.9")
    add_definitions("-g -Wall -std=c++11 -lpthread")
else()

endif()

find_package(Threads)

include_directories(server/ client/ socket/)

set(SERVER_HEAD server/server.h)
set(SERVER_SRC server/server.cpp)
set(CLIENT_HEAD client/client.h)
set(CLIENT_SRC client/client.cpp)
set(SOCKET_HEAD socket/socket.h)
set(SOCKET_SRC socket/socket.cpp)


add_library(socket ${SOCKET_HEAD} ${SOCKET_SRC})


add_executable(server ${SERVER_HEAD} ${SERVER_SRC})
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT} socket)
add_executable(client ${CLIENT_HEAD} ${CLIENT_SRC})
target_link_libraries(client socket)
