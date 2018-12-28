cmake_minimum_required(VERSION 3.12)
project(spl_net_client)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(spl_net_client
        include/connectionHandler.h
        src/connectionHandler.cpp
        src/echoClient.cpp
        makefile)
