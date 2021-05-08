cmake_minimum_required(VERSION 3.5)

project(example-server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    ../Server/TCPServer.cpp)

set(HEADERS
    ../Server/TCPServer.h
    ../Protocol/Protocol.h)

add_executable(example-server ${SOURCES} ${HEADERS})
