cmake_minimum_required(VERSION 3.12.4)
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
        src/environment/Environment.cpp
        src/environment/Environment.h
        src/modeling/Variable.cpp
        src/modeling/Variable.h
        src/utils/Exception.cpp
        src/utils/Exception.h
        src/utils/static.h
        src/modeling/Constraint.cpp
        src/modeling/Constraint.h
        src/adapters/SolverAdapter.h
        src/adapters/CplexAdapter.cpp
        src/adapters/CplexAdapter.h
        src/algorithms/DirectSolver/DirectSolver.cpp
        src/algorithms/DirectSolver/DirectSolver.h
        src/algorithms/Solver.h
        src/modeling/Expression.cpp
        src/modeling/Expression.h
        src/modeling/Objective.cpp
        src/modeling/Objective.h
        src/modeling/Model.cpp
        src/modeling/Model.h
        src/algorithms/DirectSolver/DirectMILPSolver.h
        src/algorithms/Solver.cpp
        src/algorithms/DirectSolver/DirectLPSolver.h
        src/modeling/Vector.cpp
        src/modeling/Vector.h
        src/structures/MapIterator.h
        src/algorithms/BranchAndBound/BranchAndBound.cpp
        src/algorithms/BranchAndBound/BranchAndBound.h)

if ("CPLEX" IN_LIST SOLVERS)
    target_link_libraries(lbds_solver ${CPLEX_LIBRARIES} ${CMAKE_DL_LIBS})
endif()