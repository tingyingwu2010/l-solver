project(LSolverTests)

include(../cmake/UseCPLEX.cmake)

set(PATH_TO_GOOGLE_TEST lib/googletest)

#Add the google test subdirectory
add_subdirectory(${PATH_TO_GOOGLE_TEST})

#include googletest/include dir
include_directories(${PATH_TO_GOOGLE_TEST}/googletest/include)

#include the googlemock/include dir
include_directories(${PATH_TO_GOOGLE_TEST}/googlemock/include)

add_executable(LSolverTests TestsMain.cpp modeling/VariableTests.h modeling/ExpressionTests.h algorithms/DirectLPSolver.h)
target_link_libraries(LSolverTests LSolver)
target_link_libraries(LSolverTests gtest gtest_main)
target_link_libraries(LSolverTests gmock gmock_main)
target_link_libraries(LSolverTests ${CPLEX_LIBRARIES})