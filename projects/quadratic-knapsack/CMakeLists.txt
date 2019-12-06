project(QuadraticKnapsackProject)

include(../../cmake/UseCPLEX.cmake)

add_executable(QuadraticKnapsackProject QuadraticKnapsackMain.cpp QuadraticKnapsackInstance.cpp QuadraticKnapsackInstance.h)

target_link_libraries(QuadraticKnapsackProject ${CPLEX_LIBRARIES} ${CMAKE_DL_LIBS} LSolver)