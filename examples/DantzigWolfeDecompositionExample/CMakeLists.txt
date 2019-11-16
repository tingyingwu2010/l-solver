project(DantzigWolfeDecompositionExample)

include(../../cmake/UseCPLEX.cmake)

add_executable(DantzigWolfeDecompositionExample DantzigWolfeDecompositionMain.cpp)

target_link_libraries(DantzigWolfeDecompositionExample ${CPLEX_LIBRARIES} ${CMAKE_DL_LIBS} LSolver)