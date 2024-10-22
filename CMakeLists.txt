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
        include/simulated_annealing.h
)