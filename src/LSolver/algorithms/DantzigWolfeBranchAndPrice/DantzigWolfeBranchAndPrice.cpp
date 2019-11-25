//
// Created by hlefebvr on 21/11/19.
//

#include <LSolver/adapters/CplexAdapter.h>
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


void L::DantzigWolfeBranchAndPriceNode::actually_solve_hook() {
    // DantzigWolfeDecomposition<CplexAdapter> solver(_decomposition);
    //solver.solve();

    for (const auto& var : _decomposition.source_model().variables())
        std::cout << var.user_defined_name() << " = " << var.value() << std::endl;
}
