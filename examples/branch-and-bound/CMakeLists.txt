project(BranchAndBoundKnapsackExample)

include(../../cmake/UseCPLEX.cmake)

add_executable(BranchAndBoundKnapsackExample BranchAndBoundKnapsackMain.cpp)

target_link_libraries(BranchAndBoundKnapsackExample ${CPLEX_LIBRARIES} ${CMAKE_DL_LIBS} LSolver)