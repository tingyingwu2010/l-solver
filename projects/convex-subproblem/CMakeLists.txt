project(ConvexSubproblemProject)

include(../../cmake/UseCPLEX.cmake)
add_executable(ConvexSubproblemProject ConvexSubproblemMain.cpp)

target_link_libraries(ConvexSubproblemProject ${CPLEX_LIBRARIES} ${CMAKE_DL_LIBS} LSolver -lstdc++fs)