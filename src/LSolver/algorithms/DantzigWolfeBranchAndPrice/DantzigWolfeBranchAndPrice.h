//
// Created by hlefebvr on 21/11/19.
//

#ifndef LSOLVERPROJECT_DANTZIGWOLFEBRANCHANDPRICE_H
#define LSOLVERPROJECT_DANTZIGWOLFEBRANCHANDPRICE_H

#include <LSolver/algorithms/branch-and-bound/BranchAndBound.h>
#include <LSolver/modeling/models/DualAngularModel.h>

namespace L {
    template<class Adapter_RMP, class SubProblemSolver> class DantzigWolfeBranchAndPrice;
    template<class Adapter_RMP, class SubProblemSolver> class DantzigWolfeBranchAndPriceNode;
}

template<class Adapter_RMP, class SubProblemSolver>
class L::DantzigWolfeBranchAndPrice : public BranchAndBound<DantzigWolfeBranchAndPriceNode<Adapter_RMP, SubProblemSolver>> {
    Decomposition& _decomposition;
    DantzigWolfeBranchAndPriceNode<Adapter_RMP, SubProblemSolver>* allocate_new_node() override ;
public:
    explicit DantzigWolfeBranchAndPrice(Decomposition& decomposition);
};

template<class Adapter_RMP, class SubProblemSolver>
class L::DantzigWolfeBranchAndPriceNode : public BranchAndBoundNode {
    Decomposition& _decomposition;
    void actually_solve_hook() override;
public:
    DantzigWolfeBranchAndPriceNode(const DantzigWolfeBranchAndPriceNode<Adapter_RMP, SubProblemSolver>& src);
    explicit DantzigWolfeBranchAndPriceNode(Decomposition& model);
};

#include "DantzigWolfeBranchAndPrice.cpp"

#endif //LSOLVERPROJECT_DANTZIGWOLFEBRANCHANDPRICE_H
