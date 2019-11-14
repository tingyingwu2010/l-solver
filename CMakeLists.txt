cmake_minimum_required(VERSION 3.15)
project(lbds_solver)

set(CMAKE_CXX_STANDARD 17)
set(SOLVERS "CPLEX")

# folder for cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if ("CPLEX" IN_LIST SOLVERS)
    message("Including CPLEX dependency")
    include(cmake/UseCPLEX.cmake)
endif()

add_executable(lbds_solver
        main.cpp
        environment/Environment.cpp
        environment/Environment.h
        modeling/Variable.cpp
        modeling/Variable.h
        utils/Exception.cpp
        utils/Exception.h
        utils/static.h
        modeling/Constraint.cpp
        modeling/Constraint.h
        adapters/SolverAdapter.h
        adapters/CplexAdapter.cpp
        adapters/CplexAdapter.h
        algorithms/DirectSolver/DirectSolver.cpp
        algorithms/DirectSolver/DirectSolver.h
        algorithms/Solver.h
        modeling/Expression.cpp
        modeling/Expression.h
        modeling/Objective.cpp
        modeling/Objective.h
        modeling/Model.cpp
        modeling/Model.h
        algorithms/DirectSolver/DirectMILPSolver.h
        algorithms/Solver.cpp
        algorithms/DirectSolver/DirectLPSolver.h modeling/Vector.cpp modeling/Vector.h structures/MapIterator.h)

if ("CPLEX" IN_LIST SOLVERS)
    target_link_libraries(lbds_solver ${CPLEX_LIBRARIES} ${CMAKE_DL_LIBS})
endif()