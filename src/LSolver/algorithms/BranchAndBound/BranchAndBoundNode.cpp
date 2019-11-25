//
// Created by hlefebvr on 15/11/19.
//

#include <iostream>
#include "BranchAndBoundNode.h"
#include "LSolver/modeling/objectives/Objective.h"

unsigned long int L::BranchAndBoundNode::_branch_and_bound_node_id = 0;

void L::BranchAndBoundNode::apply_bounds() {
    for (const auto& m : _variables)
        m.second->update_core_bounds();
}

void L::BranchAndBoundNode::before_actually_solving_hook() {
    apply_bounds();
}

void L::BranchAndBoundNode::before_saving_results_hook() {
    for (const auto& m : _variables) m.second->update_this_value();
}

L::BranchAndBoundNode::Solution L::BranchAndBoundNode::solution() {
    return Solution(*this);
}

void L::BranchAndBoundNode::lower_bound(const L::Variable &variable, float lb) {
    auto found = _variables.find(variable.user_defined_name());
    if (found  == _variables.end()) throw Exception("Cannot set lower bound of unknown variables");
    found->second->lb(lb);
}

void L::BranchAndBoundNode::upper_bound(const L::Variable &variable, float ub) {
    auto found = _variables.find(variable.user_defined_name());
    if (found  == _variables.end()) throw Exception("Cannot set upper bound of unknown variables");
    found->second->ub(ub);
}

L::BranchAndBoundNode::~BranchAndBoundNode() {
    for (auto& m : _variables) delete m.second;
}

L::BranchAndBoundNode::BranchAndBoundNode(const L::BranchAndBoundNode &src) : _is_feasible(false), _model(src._model) {
    for (auto& m : src._variables)
        _variables.insert({ m.first, new DetachedVariable(*m.second) });
}

L::BranchAndBoundNode::BranchAndBoundNode(L::Model &src) : _model(src) {
    for (const auto& variable : src.variables())
        _variables.insert({ variable.user_defined_name(), new DetachedVariable(variable) });
}

void L::BranchAndBoundNode::save_results_hook() {
    _objective_status = _model.objective().status();
    _objective_value = _model.objective().value();
    for (auto& ptr : _variables) {
        if (ptr.second->priority() == 0) continue;
        if (ptr.second->value() != ptr.second->ub() && ptr.second->value() != ptr.second->lb()) {
            return;
        }
    }
    _is_feasible = true;
}

void L::BranchAndBoundNode::Solution::update() {
    for (auto& m : _parent._variables) m.second->update_core_value();
    _parent._model.objective().value(_parent._objective_value);
}
