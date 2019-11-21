//
// Created by hlefebvr on 14/11/19.
//

#include "Model.h"

#include <utility>
#include <fstream>
#include "LSolver/modeling/variables/Variable.h"
#include "LSolver/modeling/constraints/Constraint.h"
#include "LSolver/modeling/expressions/Expression.h"
#include "LSolver/modeling/objectives/Objective.h"
#include "LSolver/modeling/objectives/DetachedObjective.h"
#include "../constraints/DetachedConstraint.h"
#include "../variables/DetachedVariable.h"

L::Model::Model(std::string user_defined_name) : _user_defined_name(std::move(user_defined_name)) {}

L::Model::~Model() {
    for (auto& m : _variables) delete m.second;
    for (auto& m : _constraints) delete m.second;
    delete _objective;
}

void L::Model::add(const L::Variable &variable) {
    auto found = _variables.find(variable.user_defined_name());
    if (found != _variables.end()) throw Exception("A variable has already been declared under that name: " + variable.user_defined_name());
    _variables.insert({ variable.user_defined_name(), new Variable(variable) });
}

void L::Model::add(const L::Constraint &constraint) {
    auto found = _constraints.find(constraint.user_defined_name());
    if (found != _constraints.end()) throw Exception("A constraints has already been declared under that name: " + constraint.user_defined_name());
    _constraints.insert({ constraint.user_defined_name(), new Constraint(constraint) });
}

void L::Model::add(const L::Objective &objective) {
    if (_objective != nullptr) throw Exception("Model already has an objective");
    _objective = new Objective(objective);
}

void L::Model::remove(const L::Variable &variable) {
    auto found = _variables.find(variable.user_defined_name());
    if (found == _variables.end()) throw Exception("Cannot remove non existing variable");
    _variables.erase(found);
    delete found->second;
}

void L::Model::remove(const L::Constraint &constraint) {
    auto found = _constraints.find(constraint.user_defined_name());
    if (found == _constraints.end()) throw Exception("Cannot remove non existing constraints");
    _constraints.erase(found);
    delete found->second;
}

void L::Model::remove(const L::Objective &objective) {
    if (_objective == nullptr) throw Exception("Cannot remove non exisiting objective");
    if (_objective->user_defined_name() != objective.user_defined_name())
        throw Exception("Will not remove objective because the one given differs from the present one");
    delete _objective;
    _objective = nullptr;
}

L::Model::VariableIterator L::Model::variables() {
    return VariableIterator(_variables);
}

L::Model::ConstraintIterator L::Model::constraints() {
    return ConstraintIterator(_constraints);
}

L::Objective L::Model::objective() {
    if (!_objective) throw Exception("No objective has been added");
    return Objective(*_objective);
}

std::ostream &L::operator<<(std::ostream &os, const L::Model &model) {
    os << "Model \" " + model._user_defined_name + " \": " << std::endl;
    if (model._objective) os << *model._objective << std::endl;
    else os << "(Objective) No objective" << std::endl;
    for (auto& m : model._constraints) os << *m.second << std::endl;
    for (auto& m : model._variables) os << *m.second << std::endl;
    return os;
}

const std::string &L::Model::user_defined_name() const {
    return _user_defined_name;
}

L::Constraint L::Model::constraint(const std::string name) {
    auto found = _constraints.find(name);
    if (found == _constraints.end()) throw NotFound();
    return *found->second;
}

void L::Model::add(L::VariableVector &vec) {
    for (const Variable& x : vec.components()) add(x);
}

void L::Model::add(L::ConstraintVector &vec) {
    for (const Constraint& ctr : vec.components()) add(ctr);
}

L::Variable L::Model::variable(std::string name) {
    auto found = _variables.find(name);
    if (found == _variables.end()) throw NotFound();
    return *found->second;
}
