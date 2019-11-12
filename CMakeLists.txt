cmake_minimum_required(VERSION 3.15)
project(ed_solver)

set(CMAKE_CXX_STANDARD 14)
set(SOLVERS "CPLEX")

# folder for cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if ("CPLEX" IN_LIST SOLVERS)
    message("Including CPLEX dependency")
    include(cmake/UseCPLEX.cmake)
endif()

add_executable(ed_solver main.cpp environment/Environment.cpp environment/Environment.h structures/List.cpp structures/List.h modeling/Variable.cpp modeling/Variable.h environment/AbstractEnvironmentVariable.h structures/Vector.cpp structures/Vector.h utils/Exception.cpp utils/Exception.h utils/static.h structures/Map.cpp structures/Map.h modeling/Constraint.cpp modeling/Constraint.h environment/AbstractEnvironmentConstraint.h structures/BinaryTree.h structures/BinaryTreeNode.cpp structures/BinaryTreeNode.h modeling/expressions/Expression.cpp modeling/expressions/Expression.h modeling/expressions/ExpressionNode.cpp modeling/expressions/ExpressionNode.h modeling/expressions/LinearExpression.cpp modeling/expressions/LinearExpression.h adapters/SolverAdapter.h adapters/CplexAdapter.cpp adapters/CplexAdapter.h algorithms/solvers/DirectSolver.cpp algorithms/solvers/DirectSolver.h algorithms/solvers/Solver.h)

if ("CPLEX" IN_LIST SOLVERS)
    target_link_libraries(ed_solver ${CPLEX_LIBRARIES} ${CMAKE_DL_LIBS})
endif()