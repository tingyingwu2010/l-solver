//
// Created by hlefebvr on 14/11/19.
//

#include <cmath>
#include "BranchAndBound.h"
#include "../../modeling/Objective.h"

template<class NodeClass>
void L::BranchAndBound<NodeClass>::actually_solve() {
    if (_branching_rule == nullptr) throw Exception("Cannot use branch-and-bound without branching rule.");

    while (true) {

        while (!_nodes_to_be_processed.empty()) {
            NodeClass& node = pull_node_to_be_processed();
            node.solve();
            bound(node);
        }

        fathom_dominated_nodes();
        if (!has_active_nodes()) break;
        NodeClass &node = select_node_for_branching();
        branch(node);

    }

    if (!_incumbent) { // then the problem is infeasible
        _model.objective().status(ObjectiveStatus::Infeasible);
    } else {
        _model.objective().status(ObjectiveStatus::Optimal);
        // todo copy solution informations
    }

}

template<class NodeClass>
void L::BranchAndBound<NodeClass>::branching_rule(L::BranchingRule &branching_rule) {
    _branching_rule = &branching_rule;
}

template<class NodeClass>
bool L::BranchAndBound<NodeClass>::has_active_nodes() const {
    return !_active_nodes.empty();
}

template<class NodeClass>
NodeClass &L::BranchAndBound<NodeClass>::pull_node_to_be_processed() {
    NodeClass& node = *_nodes_to_be_processed.top();
    _nodes_to_be_processed.pop();
    return node;
}

template<class NodeClass>
void L::BranchAndBound<NodeClass>::bound(NodeClass &node) {
    if (solution_improves_incumbent(node)) _incumbent = &node;
}

template<class NodeClass>
void L::BranchAndBound<NodeClass>::branch(NodeClass &node) {
    Variable branching_variable = _branching_rule(node.solution().begin(), node.solution().end());

    enum Side { Up, Down };
    auto create_node = [this, &node, &branching_variable](Side side){
        NodeClass& added_node = *new NodeClass(node);
        if (side == Up) {
            float bound = (branching_variable.type() == AbstractVariable::Binary || branching_variable.type() == AbstractVariable::Integer) ? ceil(branching_variable.value()) : branching_variable.value();
            added_node.upper_bound(branching_variable, bound);
        } else {
            float bound = (branching_variable.type() == AbstractVariable::Binary || branching_variable.type() == AbstractVariable::Integer) ? floor(branching_variable.value()) : branching_variable.value();
            added_node.lower_bound(branching_variable, bound);
        }
    };

    if (branching_variable.value() != branching_variable.ub()) create_node(Up);
    if (branching_variable.value() != branching_variable.lb()) create_node(Down);

    remove_active_node(node);
}

template<class NodeClass>
void L::BranchAndBound<NodeClass>::fathom_dominated_nodes() {
    const float tolerance = 0.000001;
    if (_incumbent == nullptr) return;
    const float incumbent_objective_value = _incumbent->objective_value();
    for (auto it = _active_nodes.cbegin(), end = _active_nodes.cend(); it != end; ++it) {
        const NodeClass &node = **it;
        if (!node.solved()) continue;
        if (node.solution().objective_value() + tolerance >= incumbent_objective_value) { // todo maximization
            _active_nodes.erase(it);
            it--;
            end--;
        }
    }
}

template<class NodeClass>
NodeClass& L::BranchAndBound<NodeClass>::select_node_for_branching() {
    float optimal = _model.objective().type() == ObjectiveType ::Maximize ? std::numeric_limits<float>::lowest() : std::numeric_limits<float>::max();
    NodeClass *branching_node = nullptr;
    for (NodeClass *node : _active_nodes) {
        if (!node->solved()) continue;
        float node_obj = node->solution().objective_value();
        if ((_model.objective().type() == ObjectiveType ::Maximize && node_obj > optimal) || (_model.objective().type() == ObjectiveType ::Minimize && node_obj < optimal)) {
            optimal = node_obj;
            branching_node = node;
        }
    }

    if (branching_node == nullptr) throw Exception("Could not find any node to branch on");
    return *branching_node;

}

template<class NodeClass>
bool L::BranchAndBound<NodeClass>::solution_improves_incumbent(NodeClass& node) {
    if (!node.solution().is_feasible()) return false;
    if (_incumbent == nullptr) return true;
    if (_model.objective().type() == ObjectiveType ::Minimize && node.solution().objective_value() < _incumbent->objective_value()) return true;
    return _model.objective().type() == ObjectiveType::Maximize && node.solution().objective_value() > _incumbent->objective_value();
}
