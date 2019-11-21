//
// Created by hlefebvr on 21/11/19.
//

#include <LSolver/algorithms/DantzigWolfeDecomposition/DantzigWolfeDecomposition.h>
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


void L::DantzigWolfeBranchAndPriceNode::actually_solve() {
    Model detached_model;
    for (auto &m : _variables) detached_model.add(Variable(*m.second));
    for (Constraint m : _model.constraints()) detached_model.add(m);
    detached_model.add(_model.objective());

    DantzigWolfeDecomposition<CplexAdapter> solver(_decomposition);
    solver.solve();
}
