//
// Created by hlefebvr on 20/11/19.
//

#include "DetachedModel.h"
#include "../constraints/DetachedConstraint.h"
#include "../objectives/Objective.h"
#include "../objectives/DetachedObjective.h"

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
    // \todo do it also for the regular variables !
    for ( auto ptr_var : _detached_variables) {
        ptr_var->update_core_value();
    }
}

void L::DetachedModel::update_objective_value() {
    // \todo do it also for the regular objective
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
