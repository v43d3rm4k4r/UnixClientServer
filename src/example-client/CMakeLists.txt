cmake_minimum_required(VERSION 3.5)

project(example-client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    ../Client/TCPClient.cpp)

set(HEADERS
    ../Client/TCPClient.h
    ../Protocol/Protocol.h)

add_executable(example-client ${SOURCES} ${HEADERS})
