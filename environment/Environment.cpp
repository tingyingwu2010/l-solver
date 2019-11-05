//
// Created by hlefebvr on 04/11/19.
//

#include "Environment.h"
#include "../modeling/Constraint.h"
#include "../modeling/ConstraintVector.h"

ED::Variable& ED::Environment::add_isolated_variable(const std::string &name) {
    auto new_variable = new Variable(name);
    _variables.add(new_variable);
    return *new_variable;
}

ED::VariableVector &ED::Environment::add_variable_vector(const std::string &name) {
    auto new_variable_vector = new VariableVector(name);
    _variable_vectors.add_component(name, *new_variable_vector);
    return *new_variable_vector;
}

ED::Constraint &ED::Environment::add_isolated_constraint(const std::string &name) {
    auto new_ctr = new Constraint(name);
    _constraints.add(new_ctr);
    return *new_ctr;
}

ED::ConstraintVector &ED::Environment::add_constraint_vector(const std::string &name) {
    auto new_ctr_vector = new ConstraintVector(name);
    _constraint_vectors.add_component(name, *new_ctr_vector);
    return *new_ctr_vector;
}
