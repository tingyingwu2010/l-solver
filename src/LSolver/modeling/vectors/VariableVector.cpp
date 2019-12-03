//
// Created by hlefebvr on 03/12/19.
//

#include "VariableVector.h"

L::VariableVector::VariableVector(L::Environment &env, const std::string &user_defined_name, unsigned long int dimension)
    : Vector<Variable>(env, user_defined_name, dimension) {}

void L::VariableVector::after_new_component_hook(L::Variable &var) {
    var.type(_type);
    var.priority(_priority);
}

void L::VariableVector::type(L::VariableType type) {
    _type = type;
    for (Variable var : components()) var.type(type);
}

void L::VariableVector::priority(unsigned int priority) {
    _priority = priority;
    for (Variable var : components()) var.priority(priority);
}
