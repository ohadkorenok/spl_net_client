cmake_minimum_required(VERSION 3.12)
project(spl_net_client)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
find_package(Boost COMPONENTS system filesystem REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++11 -pthread -lboost_thread")


add_executable(spl_net_client
        include/connectionHandler.h
        include/serverToClient.h
        include/clientToServer.h
        src/connectionHandler.cpp
        src/clientToServer.cpp
        src/serverToClient.cpp
#        src/Client.cpp
        src/echoClient.cpp)

target_link_libraries(spl_net_client ${Boost_LIBRARIES})