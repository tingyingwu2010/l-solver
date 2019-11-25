//
// Created by hlefebvr on 15/11/19.
//

#include <iostream>
#include <LSolver/algorithms/direct-solver/DirectLPSolver.h>

template<class ExternalSolver>
L::MILPBranchAndBound<ExternalSolver>::MILPBranchAndBound(L::Model &model)
    : BranchAndBound<MILPBranchAndBoundNode<ExternalSolver>>(model) {}

template<class ExternalSolver>
L::MILPBranchAndBoundNode<ExternalSolver>::MILPBranchAndBoundNode(Model& model)
    : BranchAndBoundNode(model) {}

template<class ExternalSolver>
L::MILPBranchAndBoundNode<ExternalSolver>::MILPBranchAndBoundNode(const L::MILPBranchAndBoundNode<ExternalSolver>& src)
    : BranchAndBoundNode(src) {}

template<class ExternalSolver>
L::MILPBranchAndBoundNode <ExternalSolver> *L::MILPBranchAndBound<ExternalSolver>::allocate_new_node() {
    return new MILPBranchAndBoundNode<ExternalSolver>(BranchAndBound<MILPBranchAndBoundNode<ExternalSolver>>::_model);
}

template<class ExternalSolver>
void L::MILPBranchAndBoundNode<ExternalSolver>::actually_solve_hook() {
    DirectLPSolver<ExternalSolver> solver(_model);
    solver.solve();
}
