//
// Created by hlefebvr on 15/11/19.
//

#ifndef LBDS_SOLVER_BRANCHANDBOUNDNODE_H
#define LBDS_SOLVER_BRANCHANDBOUNDNODE_H

#include <map>
#include "../Solver.h"
#include "LSolver/modeling/variables/Variable.h"
#include "../../structures/MapIterator.h"
#include "../../utils/Exception.h"
#include "LSolver/modeling/models/Model.h"
#include "LSolver/modeling/objectives/Objective.h"

namespace L {
    class BranchAndBoundNode;
}

class L::BranchAndBoundNode : public Solver {
    static unsigned long int _branch_and_bound_node_id;
protected:
    Model& _model;
    bool _is_feasible = false;
    float _objective_value = 0;
    ObjectiveStatus _objective_status = Unsolved;
    std::map<std::string, DetachedVariable*> _variables;
    const unsigned long int _id = _branch_and_bound_node_id++;
public:
    class Solution;
    friend class Solution;

    explicit BranchAndBoundNode(Model& src);
    BranchAndBoundNode(const BranchAndBoundNode& src);
    ~BranchAndBoundNode();
    virtual void upper_bound(const Variable& variable, float ub);
    virtual void lower_bound(const Variable& variable, float lb);
    virtual bool solved() const { return _objective_status != Unsolved; }
    virtual void save_results() override;
    unsigned long int id() const { return _id; }
    Solution solution();
};

class L::BranchAndBoundNode::Solution : public MapIterator<std::string, DetachedVariable, Variable> {
    const BranchAndBoundNode& _parent;
public:
    typedef MapIterator<std::string, DetachedVariable> iterator;
    explicit Solution(BranchAndBoundNode& parent) : _parent(parent), MapIterator<std::string, DetachedVariable, Variable>(parent._variables) {}
    bool is_feasible() { return _parent._is_feasible; }
    float objective_value() { return _parent._objective_value; }
    ObjectiveStatus objective_status() { return _parent._objective_status; }
    void update();
};

#endif //LBDS_SOLVER_BRANCHANDBOUNDNODE_H
