cmake_minimum_required(VERSION 3.29)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(MyProject
        src/main.cpp
        src/functions.cpp
        src/kung_solution.cpp
        src/naive_solution.cpp
)
