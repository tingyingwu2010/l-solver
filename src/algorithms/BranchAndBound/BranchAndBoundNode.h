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
    bool _solved = false;
    bool _is_feasible = false;
    std::map<std::string, DetachedVariable*> _variables;
public:
    class Solution;
    friend class Solution;

    explicit BranchAndBoundNode(Model& src);
    BranchAndBoundNode(const BranchAndBoundNode& src);
    ~BranchAndBoundNode();
    virtual void upper_bound(const Variable& variable, float ub);
    virtual void lower_bound(const Variable& variable, float lb);
    virtual bool solved() { return _solved; }
    Solution solution();
};

class L::BranchAndBoundNode::Solution : public MapIterator<std::string, DetachedVariable> {
    const BranchAndBoundNode& _parent;
public:
    typedef MapIterator<std::string, DetachedVariable> iterator;
    explicit Solution(BranchAndBoundNode& parent) : _parent(parent), MapIterator<std::string, DetachedVariable>(parent._variables) {}
    bool is_feasible() { return _parent._is_feasible; }
};

#endif //LBDS_SOLVER_BRANCHANDBOUNDNODE_H
