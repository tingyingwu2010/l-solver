project(RobustWNTJProject)

include(../../cmake/UseCPLEX.cmake)

add_executable(RobustWNTJProject RobustWNTJMain.cpp RobustWNTJInstance.cpp RobustWNTJInstance.h RobustWNTJBapSolve.h)

target_link_libraries(RobustWNTJProject ${CPLEX_LIBRARIES} ${CMAKE_DL_LIBS} LSolver)