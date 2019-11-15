//
// Created by hlefebvr on 15/11/19.
//

#include "BranchAndBoundNode.h"

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

L::BranchAndBoundNode::BranchAndBoundNode(const L::BranchAndBoundNode &src) : _solved(false), _is_feasible(false) {
    for (auto& m : src._variables)
        _variables.insert({ m.first, new DetachedVariable(*m.second) });
}

L::BranchAndBoundNode::BranchAndBoundNode(L::Model &src) {
    for (const auto& variable : src.variables())
        _variables.insert({ variable.user_defined_name(), new DetachedVariable(variable) });
}