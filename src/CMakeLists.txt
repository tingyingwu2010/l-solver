project(LSolver)

if ("CPLEX" IN_LIST SOLVERS)
    message("Including CPLEX dependency")
    include(../cmake/UseCPLEX.cmake)
endif()

add_library(LSolver
        STATIC
        LSolver/environment/Environment.cpp
        LSolver/environment/Environment.h
        LSolver/modeling/Variable.cpp
        LSolver/modeling/Variable.h
        LSolver/utils/Exception.cpp
        LSolver/utils/Exception.h
        LSolver/utils/static.h
        LSolver/modeling/Constraint.cpp
        LSolver/modeling/Constraint.h
        LSolver/adapters/SolverAdapter.h
        LSolver/adapters/CplexAdapter.cpp
        LSolver/adapters/CplexAdapter.h
        LSolver/algorithms/DirectSolver/DirectSolver.cpp
        LSolver/algorithms/DirectSolver/DirectSolver.h
        LSolver/algorithms/Solver.h
        LSolver/modeling/Expression.cpp
        LSolver/modeling/Expression.h
        LSolver/modeling/Objective.cpp
        LSolver/modeling/Objective.h
        LSolver/modeling/Model.cpp
        LSolver/modeling/Model.h
        LSolver/algorithms/DirectSolver/DirectMILPSolver.h
        LSolver/algorithms/Solver.cpp
        LSolver/algorithms/DirectSolver/DirectLPSolver.h
        LSolver/modeling/Vector.cpp
        LSolver/modeling/Vector.h
        LSolver/structures/MapIterator.h
        LSolver/algorithms/BranchAndBound/BranchAndBound.h
        LSolver/algorithms/BranchAndBound/BranchingRules/BranchingRule.h
        LSolver/algorithms/BranchAndBound/BranchingRules/MostInfeasibleBranchingRule.cpp
        LSolver/algorithms/BranchAndBound/BranchingRules/MostInfeasibleBranchingRule.h
        LSolver/algorithms/BranchAndBound/BranchAndBoundNode.h
        LSolver/algorithms/BranchAndBound/BranchAndBoundNode.cpp
        LSolver/algorithms/MILPBranchAndBound/MILPBranchAndBound.h
        LSolver/application/Application.cpp
        LSolver/application/Application.h
        LSolver/application/ParameterManager.cpp
        LSolver/application/ParameterManager.h
        LSolver/application/LogManager.h
        LSolver/application/enums.h
        LSolver/algorithms/DantzigWolfeDecomposition/DantzigWolfeDecomposition.h LSolver/modeling/DualAngularModel.cpp LSolver/modeling/DualAngularModel.h LSolver/modeling/Row.h)

if ("CPLEX" IN_LIST SOLVERS)
    target_link_libraries(LSolver ${CPLEX_LIBRARIES} ${CMAKE_DL_LIBS})
endif()