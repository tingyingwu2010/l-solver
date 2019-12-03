//
// Created by hlefebvr on 21/11/19.
//

#include <LSolver/algorithms/dantzig-wolfe/DantzigWolfeDecompositionSolver.h>

template<class Adapter_RMP, class SubProblemSolver>
L::DantzigWolfeBranchAndPrice<Adapter_RMP, SubProblemSolver>::DantzigWolfeBranchAndPrice(L::Decomposition &decomposition)
    : BranchAndBound<DantzigWolfeBranchAndPriceNode<Adapter_RMP, SubProblemSolver>>(decomposition.source_model()),
    _decomposition(decomposition) {}

template<class Adapter_RMP, class SubProblemSolver>
L::DantzigWolfeBranchAndPriceNode<Adapter_RMP, SubProblemSolver> *L::DantzigWolfeBranchAndPrice<Adapter_RMP, SubProblemSolver>::allocate_new_node() {
    return new DantzigWolfeBranchAndPriceNode<Adapter_RMP, SubProblemSolver>(_decomposition);
}

template<class Adapter_RMP, class SubProblemSolver>
L::DantzigWolfeBranchAndPriceNode<Adapter_RMP, SubProblemSolver>::DantzigWolfeBranchAndPriceNode(const L::DantzigWolfeBranchAndPriceNode<Adapter_RMP, SubProblemSolver> &src)
    : BranchAndBoundNode(src), _decomposition(src._decomposition) {}

template<class Adapter_RMP, class SubProblemSolver>
L::DantzigWolfeBranchAndPriceNode<Adapter_RMP, SubProblemSolver>::DantzigWolfeBranchAndPriceNode(L::Decomposition &decomposition)
    : BranchAndBoundNode(decomposition.source_model()), _decomposition(decomposition) {}

template<class Adapter_RMP, class SubProblemSolver>
void L::DantzigWolfeBranchAndPriceNode<Adapter_RMP, SubProblemSolver>::actually_solve_hook() {
    DantzigWolfeDecompositionSolver<Adapter_RMP, SubProblemSolver> solver(_decomposition);
    solver.solve();
}
