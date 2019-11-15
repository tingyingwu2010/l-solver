//
// Created by hlefebvr on 15/11/19.
//

#ifndef LBDS_SOLVER_BRANCHANDBOUNDNODE_H
#define LBDS_SOLVER_BRANCHANDBOUNDNODE_H

#include <map>
#include "../Solver.h"
#include "../../modeling/Variable.h"
#include "../../structures/MapIterator.h"
#include "../../utils/Exception.h"
#include "../../modeling/Model.h"

namespace L {
    class BranchAndBoundNode;
}

class L::BranchAndBoundNode : public Solver {
protected:
    Model& _model;
    bool _solved = false;
    bool _is_feasible = false;
    float _objective_value = 0;
    std::map<std::string, DetachedVariable*> _variables;
public:
    class Solution;
    friend class Solution;

    explicit BranchAndBoundNode(Model& src);
    BranchAndBoundNode(const BranchAndBoundNode& src);
    ~BranchAndBoundNode();
    virtual void upper_bound(const Variable& variable, float ub);
    virtual void lower_bound(const Variable& variable, float lb);
    virtual bool solved() const { return _solved; }
    virtual void save_results() override;
    Solution solution();
};

class L::BranchAndBoundNode::Solution : public MapIterator<std::string, DetachedVariable, Variable> {
    const BranchAndBoundNode& _parent;
public:
    typedef MapIterator<std::string, DetachedVariable> iterator;
    explicit Solution(BranchAndBoundNode& parent) : _parent(parent), MapIterator<std::string, DetachedVariable, Variable>(parent._variables) {}
    bool is_feasible() { return _parent._is_feasible; }
    float objective_value() { return _parent._objective_value; }
    void update();
};

#endif //LBDS_SOLVER_BRANCHANDBOUNDNODE_H
