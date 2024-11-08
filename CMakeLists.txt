cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(MyProject
        src/main.cpp
        src/distributions.cpp
        src/circle.cpp
        src/local_search.cpp
        src/simulated_annealing.cpp
        src/functions.cpp
        src/Point.cpp
        src/evolutionary_algorithm.cpp
        src/evolutionary_algorithm.cpp
        src/evolutionary_test_int.cpp
        include/evolutionary_test_int.h
)