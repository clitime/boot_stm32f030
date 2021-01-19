cmake_minimum_required(VERSION 3.17)
project(test)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    ../src/queue.hpp
    ../src/protocol/protocol.cpp
    test_main.cpp
    test_uqueue.cpp
    test_protocol.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ../src/
    ../src/protocol)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE
    GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
