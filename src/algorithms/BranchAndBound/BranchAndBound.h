//
// Created by hlefebvr on 14/11/19.
//

#ifndef LBDS_SOLVER_BRANCHANDBOUND_H
#define LBDS_SOLVER_BRANCHANDBOUND_H

#include <functional>
#include "../Solver.h"
#include "../../modeling/Variable.h"

namespace L {
    class BranchAndBound;
}

namespace L::BranchingRule {
    // most infeasible
    // random branching
    // strong branching
}

class L::BranchAndBound : public Solver {
    const std::function<void()>& _branching_rule;
    const std::function<void()>& _solve_node;
public:

};


#endif //LBDS_SOLVER_BRANCHANDBOUND_H
