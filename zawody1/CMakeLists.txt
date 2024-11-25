cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(MyProject
        src/main.cpp
        src/Float_representation.cpp
        src/ecdf_summary.cpp
        src/operators_and_selection.cpp
        src/evolutionary_algorithm.cpp
        src/eval_experiments.cpp
)