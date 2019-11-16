//
// Created by hlefebvr on 14/11/19.
//

#include "Model.h"

#include <utility>
#include <fstream>
#include "Variable.h"
#include "Constraint.h"
#include "Expression.h"
#include "Objective.h"

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
    if (found != _constraints.end()) throw Exception("A constraint has already been declared under that name: " + constraint.user_defined_name());
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
    if (found == _constraints.end()) throw Exception("Cannot remove non existing constraint");
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
    return Objective(*_objective);
}
