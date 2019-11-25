//
// Created by hlefebvr on 15/11/19.
//

#ifndef LBDS_SOLVER_MILPBRANCHANDBOUND_H
#define LBDS_SOLVER_MILPBRANCHANDBOUND_H

#include "../BranchAndBound/BranchAndBoundNode.h"
#include "../BranchAndBound/BranchAndBound.h"

namespace L {
    template<class ExternalSolver> class MILPBranchAndBoundNode;
    template<class ExternalSolver> class MILPBranchAndBound;
}

template<class ExternalSolver>
class L::MILPBranchAndBound : public BranchAndBound<MILPBranchAndBoundNode<ExternalSolver>> {
    MILPBranchAndBoundNode<ExternalSolver>* allocate_new_node() override;
public:
    explicit MILPBranchAndBound(Model& model);
};

template<class ExternalSolver>
class L::MILPBranchAndBoundNode : public BranchAndBoundNode {
    void actually_solve_hook() override;
public:
    MILPBranchAndBoundNode(const MILPBranchAndBoundNode& src);
    explicit MILPBranchAndBoundNode(Model& model);
};

#include "MILPBranchAndBound.cpp"

#endif //LBDS_SOLVER_MILPBRANCHANDBOUND_H
