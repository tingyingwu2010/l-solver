cmake_minimum_required(VERSION 3.15)
project(ed_solver)

set(CMAKE_CXX_STANDARD 14)

add_executable(ed_solver main.cpp environment/Environment.cpp environment/Environment.h structures/List.cpp structures/List.h modeling/Variable.cpp modeling/Variable.h environment/AbstractEnvironmentVariable.cpp environment/AbstractEnvironmentVariable.h structures/Vector.cpp structures/Vector.h modeling/VariableVector.cpp modeling/VariableVector.h utils/Exception.cpp utils/Exception.h utils/static.h structures/Map.cpp structures/Map.h modeling/Expression.cpp modeling/Expression.h)