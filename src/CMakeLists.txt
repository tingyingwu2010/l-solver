project(LSolver)

if ("CPLEX" IN_LIST SOLVERS)
    message("Including CPLEX dependency")
    include(../cmake/UseCPLEX.cmake)
endif()

add_library(LSolver
        STATIC
        LSolver/environment/Environment.cpp
        LSolver/environment/Environment.h
        LSolver/modeling/variables/Variable.cpp
        LSolver/modeling/variables/Variable.h
        LSolver/utils/Exception.cpp
        LSolver/utils/Exception.h
        LSolver/utils/static.h
        LSolver/modeling/constraints/Constraint.cpp
        LSolver/modeling/constraints/Constraint.h
        LSolver/adapters/SolverAdapter.h
        LSolver/adapters/CplexAdapter.cpp
        LSolver/adapters/CplexAdapter.h
        LSolver/algorithms/direct-solver/DirectSolver.cpp
        LSolver/algorithms/direct-solver/DirectSolver.h
        LSolver/algorithms/Solver.h
        LSolver/modeling/expressions/Expression.cpp
        LSolver/modeling/expressions/Expression.h
        LSolver/modeling/objectives/Objective.cpp
        LSolver/modeling/objectives/Objective.h
        LSolver/modeling/models/Model.cpp
        LSolver/modeling/models/Model.h
        LSolver/algorithms/direct-solver/DirectMILPSolver.h
        LSolver/algorithms/Solver.cpp
        LSolver/algorithms/direct-solver/DirectLPSolver.h
        LSolver/modeling/vectors/Vector.h
        LSolver/structures/MapIterator.h
        LSolver/algorithms/branch-and-bound/BranchAndBound.h
        LSolver/algorithms/branch-and-bound/branching-rules/BranchingRule.h
        LSolver/algorithms/branch-and-bound/branching-rules/MostInfeasibleBranchingRule.cpp
        LSolver/algorithms/branch-and-bound/branching-rules/MostInfeasibleBranchingRule.h
        LSolver/algorithms/branch-and-bound/BranchAndBoundNode.h
        LSolver/algorithms/branch-and-bound/BranchAndBoundNode.cpp
        LSolver/algorithms/milp-branch-and-bound/MILPBranchAndBound.h
        LSolver/application/Application.cpp
        LSolver/application/Application.h
        LSolver/application/ParameterManager.cpp
        LSolver/application/ParameterManager.h
        LSolver/application/LogManager.h
        LSolver/application/enums.h
        LSolver/algorithms/column-generation/ColumnGeneration.h
        LSolver/algorithms/column-generation/Column.cpp
        LSolver/algorithms/column-generation/Column.h
        LSolver/algorithms/column-generation/ColumnIterator.h
        LSolver/modeling/constraints/AbstractConstraint.h
        LSolver/modeling/enums.h
        LSolver/modeling/constraints/AbstractConstraint.cpp
        LSolver/modeling/constraints/CoreConstraint.cpp
        LSolver/modeling/constraints/CoreConstraint.h
        LSolver/modeling/constraints/DetachedConstraint.cpp
        LSolver/modeling/constraints/DetachedConstraint.h
        LSolver/modeling/objectives/AbstractObjective.cpp
        LSolver/modeling/objectives/AbstractObjective.h
        LSolver/modeling/enums.cpp
        LSolver/modeling/objectives/DetachedObjective.cpp
        LSolver/modeling/objectives/DetachedObjective.h
        LSolver/modeling/objectives/CoreObjective.cpp
        LSolver/modeling/objectives/CoreObjective.h
        LSolver/modeling/variables/AbstractVariable.cpp
        LSolver/modeling/variables/AbstractVariable.h
        LSolver/modeling/variables/CoreVariable.cpp
        LSolver/modeling/variables/CoreVariable.h
        LSolver/modeling/variables/DetachedVariable.cpp
        LSolver/modeling/variables/DetachedVariable.h
        LSolver/algorithms/dantzig-wolfe-branch-and-price/DantzigWolfeBranchAndPrice.h
        LSolver/modeling/models/Decomposition.cpp
        LSolver/modeling/models/Decomposition.h
        LSolver/algorithms/dantzig-wolfe/DantzigWolfeModel.cpp
        LSolver/algorithms/dantzig-wolfe/DantzigWolfeModel.h
        LSolver/algorithms/dantzig-wolfe/DantzigWolfeDecompositionAlgorithm.h
        LSolver/algorithms/dantzig-wolfe/DantzigWolfeColumnIterator.h
        LSolver/algorithms/dantzig-wolfe/DantzigWolfeDecompositionSolver.h ../projects/robust-middleman-problem/RobustMiddleManMain.cpp LSolver/modeling/vectors/VariableVector.cpp LSolver/modeling/vectors/VariableVector.h LSolver/plots/Plot.cpp LSolver/plots/Plot.h LSolver/plots/SubPlot.cpp LSolver/plots/SubPlot.h LSolver/plots/Surface.cpp LSolver/plots/Surface.h LSolver/plots/MeshPoints.cpp LSolver/plots/MeshPoints.h)

if ("CPLEX" IN_LIST SOLVERS)
    target_link_libraries(LSolver ${CPLEX_LIBRARIES} ${CMAKE_DL_LIBS})
endif()