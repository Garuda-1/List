cmake_minimum_required(VERSION 3.13)
project(exam_list)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -Wall -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined -fsanitize=address -fsanitize=leak -Wall -std=c++11 -pedantic -D_GLIBCXX_DEBUG")
endif()

set(CMAKE_CXX_STANDARD 14)

add_executable(exam_list_testing
        gtest/gtest.h
        gtest/gtest-all.cc
        gtest/gtest_main.cc

        list.hpp

        counted.h
        counted.cpp
        fault_injection.h
        fault_injection.cpp

        tests.cpp
        )

target_link_libraries(exam_list_testing pthread)