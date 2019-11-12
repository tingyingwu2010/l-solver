cmake_minimum_required(VERSION 3.15)
project(ed_solver)

set(CMAKE_CXX_STANDARD 14)

add_executable(ed_solver main.cpp environment/Environment.cpp environment/Environment.h structures/List.cpp structures/List.h modeling/Variable.cpp modeling/Variable.h environment/AbstractEnvironmentVariable.h structures/Vector.cpp structures/Vector.h utils/Exception.cpp utils/Exception.h utils/static.h structures/Map.cpp structures/Map.h modeling/Constraint.cpp modeling/Constraint.h environment/AbstractEnvironmentConstraint.h structures/BinaryTree.h structures/BinaryTreeNode.cpp structures/BinaryTreeNode.h modeling/expressions/Expression.cpp modeling/expressions/Expression.h modeling/expressions/ExpressionNode.cpp modeling/expressions/ExpressionNode.h modeling/expressions/LinearExpression.cpp modeling/expressions/LinearExpression.h)