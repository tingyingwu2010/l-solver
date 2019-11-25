//
// Created by hlefebvr on 21/11/19.
//

#ifndef LSOLVERPROJECT_DANTZIGWOLFEBRANCHANDPRICE_H
#define LSOLVERPROJECT_DANTZIGWOLFEBRANCHANDPRICE_H

#include <LSolver/algorithms/BranchAndBound/BranchAndBound.h>
#include <LSolver/modeling/models/DualAngularModel.h>

namespace L {
    class DantzigWolfeBranchAndPrice;
    class DantzigWolfeBranchAndPriceNode;
}

class L::DantzigWolfeBranchAndPrice : public BranchAndBound<DantzigWolfeBranchAndPriceNode> {
    Decomposition& _decomposition;
    DantzigWolfeBranchAndPriceNode* allocate_new_node() override ;
public:
    explicit DantzigWolfeBranchAndPrice(Decomposition& decomposition);
};

class L::DantzigWolfeBranchAndPriceNode : public BranchAndBoundNode {
    Decomposition& _decomposition;
    void actually_solve_hook() override;
public:
    DantzigWolfeBranchAndPriceNode(const DantzigWolfeBranchAndPriceNode& src);
    explicit DantzigWolfeBranchAndPriceNode(Decomposition& model);
};


#endif //LSOLVERPROJECT_DANTZIGWOLFEBRANCHANDPRICE_H
