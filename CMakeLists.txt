cmake_minimum_required(VERSION 3.15)
project(LSolverProject)

set(CMAKE_CXX_STANDARD 17)
set(SOLVERS "CPLEX")

# folder for cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include_directories(src)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tests)
