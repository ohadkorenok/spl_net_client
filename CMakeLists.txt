cmake_minimum_required(VERSION 3.12)
project(spl_net_client)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(spl_net_client
        include/connectionHandler.h
        src/connectionHandler.cpp
        src/echoClient.cpp
        makefile include/serverToClient.h include/clientToServer.h src/clientToServer.cpp src/serverToClient.cpp)
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})