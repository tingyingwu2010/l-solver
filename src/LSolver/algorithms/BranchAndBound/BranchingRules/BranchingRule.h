//
// Created by hlefebvr on 15/11/19.
//

#ifndef LBDS_SOLVER_BRANCHINGRULE_H
#define LBDS_SOLVER_BRANCHINGRULE_H

#include "../../../modeling/Model.h"
#include "../../../modeling/Variable.h"
#include "../BranchAndBoundNode.h"

namespace L {
    class BranchingRule;
}

class L::BranchingRule {
public:
    virtual Variable operator()(BranchAndBoundNode&) = 0;
};

#endif //LBDS_SOLVER_BRANCHINGRULE_H
