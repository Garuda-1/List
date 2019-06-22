cmake_minimum_required(VERSION 3.13)
project(exam_list)

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