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
        LSolver/algorithms/DirectSolver/DirectSolver.cpp
        LSolver/algorithms/DirectSolver/DirectSolver.h
        LSolver/algorithms/Solver.h
        LSolver/modeling/expressions/Expression.cpp
        LSolver/modeling/expressions/Expression.h
        LSolver/modeling/objectives/Objective.cpp
        LSolver/modeling/objectives/Objective.h
        LSolver/modeling/models/Model.cpp
        LSolver/modeling/models/Model.h
        LSolver/algorithms/DirectSolver/DirectMILPSolver.h
        LSolver/algorithms/Solver.cpp
        LSolver/algorithms/DirectSolver/DirectLPSolver.h
        LSolver/modeling/vectors/Vector.h
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
        LSolver/modeling/models/DualAngularModel.cpp
        LSolver/modeling/models/DualAngularModel.h
        LSolver/algorithms/ColumnGeneration/ColumnGeneration.h
        LSolver/algorithms/DantzigWolfeDecomposition/DantzigWolfeDecomposition.h
        LSolver/algorithms/ColumnGeneration/Column.cpp
        LSolver/algorithms/ColumnGeneration/Column.h
        LSolver/algorithms/ColumnGeneration/ColumnIterator.h
        LSolver/algorithms/DantzigWolfeDecomposition/DantzigWolfeColumnIterator.h
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
        LSolver/modeling/variables/DetachedVariable.h LSolver/modeling/models/DetachedModel.cpp LSolver/modeling/models/DetachedModel.h)

if ("CPLEX" IN_LIST SOLVERS)
    target_link_libraries(LSolver ${CPLEX_LIBRARIES} ${CMAKE_DL_LIBS})
endif()