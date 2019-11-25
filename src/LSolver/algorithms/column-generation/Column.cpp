//
// Created by hlefebvr on 20/11/19.
//

#include <LSolver/modeling/constraints/Constraint.h>
#include "Column.h"

const L::Column::CoefficientsAsConstraints &L::Column::as_constraints() const {
    return _as_constraints;
}

float L::Column::reduced_cost() const {
    return _reduced_cost;
}

void L::Column::reduced_cost(float r) {
    _reduced_cost = r;
}

void L::Column::constraint_coefficient(const Constraint &ctr, float coef) {
    _as_constraints.emplace_back(std::pair<Constraint, float>(ctr, coef ));
}

void L::Column::objective_cost(float c) {
    _objective_cost = c;
}

float L::Column::objective_cost() const {
    return _objective_cost;
}

bool L::Column::empty() const {
    return _as_constraints.empty();
}

const L::Column::CoefficientsAsVariables &L::Column::as_variables() const {
    return _as_variables;
}

void L::Column::variable_coefficient(const Variable &ctr, float coef) {
    _as_variables.emplace_back(std::pair<Variable, float>(ctr, coef ));
}
