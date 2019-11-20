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

unsigned int L::Model::user_argument(unsigned int index) const {
    auto found = _user_arguments.find(index);
    if (found == _user_arguments.end()) return 0;
    return found->second;
}

void L::Model::user_argument(unsigned int index, unsigned int value) {
    auto found = _user_arguments.find(index);
    if (found == _user_arguments.end() && value != 0) _user_arguments.insert({ index, value });
    else if (value == 0) _user_arguments.erase(found);
    else found->second = value;
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

////////////// DETACHED MODEL

L::DetachedModel::DetachedModel(L::Model &src) : Model(src.user_defined_name()), _src(src) {
    for (const Variable& variable : _src.variables()) add_detached(variable);
    for (const Constraint& ctr : _src.constraints()) add_detached(ctr);
    add_detached(_src.objective());
}

L::DetachedModel::~DetachedModel() {
    for (auto ptr : _detached_variables) delete ptr;
    for (auto ptr : _detached_constraints) delete ptr;
    delete _detached_objective;
}

void L::DetachedModel::add_detached(const L::Variable &variable) {
    DetachedVariable& dvar = *new DetachedVariable(variable);
    _detached_variables.emplace_back(&dvar);
    Model::add(Variable(dvar));
}

void L::DetachedModel::add_detached(const L::Constraint &ctr) {
    DetachedConstraint& dctr = *new DetachedConstraint(ctr, false);
    _detached_constraints.emplace_back(&dctr);
    Model::add(Constraint(dctr));
}

void L::DetachedModel::add_detached(const L::Objective &obj) {
    if (_detached_objective) throw Exception("(Detached) Model already has an objective");
    DetachedObjective& dobj = *new DetachedObjective(obj);
    _detached_objective = &dobj;
    Model::add(Objective(dobj));
}

void L::DetachedModel::update_primal_values() {
    for ( auto ptr_var : _detached_variables) {
        ptr_var->update_core_value();
    }
}

void L::DetachedModel::update_objective_value() {
    _detached_objective->update_core_value();
}

void L::DetachedModel::add(L::DetachedVariable &variable) {
    Model::add(Variable(variable));
}

void L::DetachedModel::add(L::DetachedConstraint &ctr) {
    Model::add(Constraint(ctr));
}

void L::DetachedModel::add(L::DetachedObjective &obj) {
    Model::add(Objective(obj));
}


