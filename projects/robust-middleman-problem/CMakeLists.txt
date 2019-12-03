project(RobustMiddleManProject)

include(../../cmake/UseCPLEX.cmake)

add_executable(RobustMiddleManProject RobustMiddleManMain.cpp RobustMiddlemanInstance.cpp RobustMiddlemanInstance.h)

target_link_libraries(RobustMiddleManProject ${CPLEX_LIBRARIES} ${CMAKE_DL_LIBS} LSolver)