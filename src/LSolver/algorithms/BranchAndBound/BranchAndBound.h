//
// Created by hlefebvr on 14/11/19.
//

#ifndef LBDS_SOLVER_BRANCHANDBOUND_H
#define LBDS_SOLVER_BRANCHANDBOUND_H

#include <functional>
#include <stack>
#include "../Solver.h"
#include "../../modeling/Variable.h"
#include "BranchingRules/BranchingRule.h"

namespace L {
    template<class NodeClass> class BranchAndBound;
}

template<class NodeClass>
class L::BranchAndBound : public Solver {
protected:
    Model& _model;
    BranchingRule* _branching_rule = nullptr;
    std::vector<NodeClass*> _active_nodes;
    std::stack<NodeClass*> _nodes_to_be_processed;
    NodeClass* _incumbent = nullptr;
    bool _unbounded = false;
    void actually_solve() override;

    bool has_active_nodes() const;
    NodeClass& pull_node_to_be_processed();
    void branch(NodeClass& node);
    void bound(NodeClass& node);
    void fathom_dominated_nodes();
    NodeClass& select_node_for_branching();
    bool solution_improves_incumbent(NodeClass& node);
    void remove_active_node(const NodeClass& node);
public:
    explicit BranchAndBound(Model& model);
    void branching_rule(BranchingRule& branching_rule);
    void save_results() override;
};

#include "BranchAndBound.cpp"

#endif //LBDS_SOLVER_BRANCHANDBOUND_H
