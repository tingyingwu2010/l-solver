//
// Created by hlefebvr on 14/11/19.
//

#include <cmath>
#include <LSolver/application/LogManager.h>
#include "../../application/Application.h"


template<class NodeClass>
void L::BranchAndBound<NodeClass>::actually_solve() {
    if (_branching_rule == nullptr) throw Exception("Cannot use branch-and-bound without branching rule.");

    NodeClass& root_node = *allocate_new_node();
    _active_nodes.emplace_back(&root_node);
    _nodes_to_be_processed.push(&root_node);

    while (true) {

        while (!_nodes_to_be_processed.empty()) {
            NodeClass& node = pull_node_to_be_processed();
            _L_LOG_(Debug) << "Solving node " << node.id() << "..." << std::endl;
            node.solve();
            _L_LOG_(Release) << "Node " << node.id() << " was solved in " << node.last_execution_time()
                << "s, and ended with status " << node.solution().objective_status()
                << ", vertex: "
                << node.solution().is_feasible() << ", LB = " << node.solution().objective_value() << "" << std::endl;
            if (node.solution().objective_status() == Unbounded) { _unbounded = true; return; }
            bound(node);
        }

        fathom_dominated_nodes();
        if (!has_active_nodes()) break;
        NodeClass &node = select_node_for_branching();
        branch(node);

    }

}

template<class NodeClass>
void L::BranchAndBound<NodeClass>::branching_rule(L::BranchingRule &branching_rule) {
    _branching_rule = &branching_rule;
    _L_LOG_(Debug) << "Branching rule has been set with " << typeid(branching_rule).name() << "" << std::endl;
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
    if (solution_improves_incumbent(node)) {
        _incumbent = &node;
        _L_LOG_(Release) << "New incumbent found, UB = " << _incumbent->solution().objective_value() << "" << std::endl;
    }
}

template<class NodeClass>
void L::BranchAndBound<NodeClass>::branch(NodeClass &node) {
    if (node.solution().objective_status() == Infeasible) return;

    Variable branching_variable = (*_branching_rule)(node);
    _L_LOG_(Debug) << "Branching rule selected variable " << branching_variable.user_defined_name() << ", current bounds: "
        << branching_variable.lb() << ", " << branching_variable.ub() << ", value = " << branching_variable.value() << "" << std::endl;
    if (branching_variable.value() != branching_variable.ub() && branching_variable.value() != branching_variable.lb()) {

        enum Side { Up, Down };
        auto create_node = [this, &node, &branching_variable](Side side) {
            NodeClass &added_node = *new NodeClass(node);
            if (side == Up) {
                float bound = (branching_variable.type() == Binary ||
                               branching_variable.type() == Integer) ? ceil(
                        branching_variable.value()) : branching_variable.value();
                added_node.lower_bound(branching_variable, bound);
                _L_LOG_(Release) << "Branching. Created node " << added_node.id() << " with additional bounds " << bound << " <= "
                    << branching_variable.user_defined_name() << " <= " << branching_variable.ub() << "" << std::endl;
            } else {
                float bound = (branching_variable.type() == Binary ||
                               branching_variable.type() == Integer) ? floor(
                        branching_variable.value()) : branching_variable.value();
                added_node.upper_bound(branching_variable, bound);
                _L_LOG_(Release) << "Branching. Created node " << added_node.id() << " with additional bounds " << branching_variable.lb()
                    << " <= " << branching_variable.user_defined_name() << " <= " << bound << "" << std::endl;
            }
            _nodes_to_be_processed.push(&added_node);
            _active_nodes.emplace_back(&added_node);
        };

        if (branching_variable.value() != branching_variable.ub()) create_node(Up);
        if (branching_variable.value() != branching_variable.lb()) create_node(Down);

    }

    remove_active_node(node);
    _L_LOG_(Debug) << "Node " << node.id() << " was removed from active nodes because it was branched uppon" << std::endl;
}

template<class NodeClass>
void L::BranchAndBound<NodeClass>::fathom_dominated_nodes() {
    const float tolerance = Application::parameters().tolerance();

    if (!_incumbent) {
        _L_LOG_(Debug) << "No incumbent has been found so far, only fathoming infeasible nodes (if any)" << std::endl;
    }

    const float incumbent_objective_value = _incumbent ? _incumbent->solution().objective_value() : std::numeric_limits<float>::max();
    for (auto it = _active_nodes.cbegin(), end = _active_nodes.cend(); it != end; ++it) {
        NodeClass &node = **it;
        if (!node.solved()) continue;
        if (node.solution().objective_status() == Infeasible || node.solution().objective_value() + tolerance >= incumbent_objective_value) {
            _active_nodes.erase(it);
            _L_LOG_(Release) << "Node " << node.id() << " was fathomed because LB = " << node.solution().objective_value() << " >= " << incumbent_objective_value << " = UB" << std::endl;
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
    _L_LOG_(Debug) << "Node " << branching_node->id() << " has been selected for branching" << std::endl;
    return *branching_node;
}

template<class NodeClass>
bool L::BranchAndBound<NodeClass>::solution_improves_incumbent(NodeClass& node) {
    if (!node.solution().is_feasible()) return false;
    if (_incumbent == nullptr) return true;
    if (_model.objective().type() == ObjectiveType ::Minimize && node.solution().objective_value() < _incumbent->solution().objective_value()) return true;
    return _model.objective().type() == ObjectiveType::Maximize && node.solution().objective_value() > _incumbent->solution().objective_value();
}

template<class NodeClass>
void L::BranchAndBound<NodeClass>::save_results() {

    if (_unbounded) {
        _model.objective().status(Unbounded);
        _model.objective().value(std::numeric_limits<float>::lowest());
        _L_LOG_(Release) << "Problem was found to be unbounded" << std::endl;
        return;
    }

    if (!_incumbent) {
        _model.objective().status(Infeasible);
        _L_LOG_(Release) << "No feasible solution was found" << std::endl;
        return;
    }

    _L_LOG_(Release) << "Optimal solution found ! Updating core variables..." << std::endl;
    _incumbent->solution().update();
}

template<class NodeClass>
L::BranchAndBound<NodeClass>::BranchAndBound(L::Model &model) : _model(model) {}

template<class NodeClass>
void L::BranchAndBound<NodeClass>::remove_active_node(const NodeClass &node) {
    for (auto it = _active_nodes.begin(), end = _active_nodes.end() ; it != end ; ++it) {
        if (*it == &node) {
            _active_nodes.erase(it);
            break;
        }
    }
}
