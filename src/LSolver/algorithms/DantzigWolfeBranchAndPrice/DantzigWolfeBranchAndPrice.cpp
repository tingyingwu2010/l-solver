//
// Created by hlefebvr on 21/11/19.
//

#include "DantzigWolfeBranchAndPrice.h"

L::DantzigWolfeBranchAndPrice::DantzigWolfeBranchAndPrice(L::Decomposition &decomposition)
    : BranchAndBound(decomposition.source_model()), _decomposition(decomposition) {}

L::DantzigWolfeBranchAndPriceNode *L::DantzigWolfeBranchAndPrice::allocate_new_node() {
    return new DantzigWolfeBranchAndPriceNode(_decomposition);
}


L::DantzigWolfeBranchAndPriceNode::DantzigWolfeBranchAndPriceNode(const L::DantzigWolfeBranchAndPriceNode &src)
    : BranchAndBoundNode(src), _decomposition(src._decomposition) {}

L::DantzigWolfeBranchAndPriceNode::DantzigWolfeBranchAndPriceNode(L::Decomposition &decomposition)
    : BranchAndBoundNode(decomposition.source_model()), _decomposition(decomposition) {}


void L::DantzigWolfeBranchAndPriceNode::actually_solve() {
    // do it
}
