//
// Created by hlefebvr on 15/11/19.
//

#ifndef LBDS_SOLVER_BRANCHINGRULE_H
#define LBDS_SOLVER_BRANCHINGRULE_H

#include "../../../modeling/Model.h"
#include "../../../modeling/Variable.h"

namespace L {
    class BranchingRule;
}

class L::BranchingRule {
public:
    virtual Variable operator()(Model::VariableIterator begin, Model::VariableIterator end) = 0;
};

#endif //LBDS_SOLVER_BRANCHINGRULE_H
