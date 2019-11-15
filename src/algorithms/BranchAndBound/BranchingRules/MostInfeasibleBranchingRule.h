//
// Created by hlefebvr on 15/11/19.
//

#ifndef LBDS_SOLVER_MOSTINFEASIBLEBRANCHINGRULE_H
#define LBDS_SOLVER_MOSTINFEASIBLEBRANCHINGRULE_H

#include "BranchingRule.h"
#include "../BranchAndBoundNode.h"

namespace L {
    class MostInfeasibleBranchingRule;
}


class L::MostInfeasibleBranchingRule : public BranchingRule {
    Variable operator()(BranchAndBoundNode&);
};


#endif //LBDS_SOLVER_MOSTINFEASIBLEBRANCHINGRULE_H
