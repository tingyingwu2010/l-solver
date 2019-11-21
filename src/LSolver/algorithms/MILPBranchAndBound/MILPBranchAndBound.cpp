//
// Created by hlefebvr on 15/11/19.
//

#include <iostream>
#include "../DirectSolver/DirectLPSolver.h"

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
void L::MILPBranchAndBoundNode<ExternalSolver>::actually_solve() {
    Model detached_model;
    for (auto &m : _variables) detached_model.add(Variable(*m.second));
    for (Constraint m : _model.constraints()) detached_model.add(m);
    detached_model.add(_model.objective());

    DirectLPSolver<ExternalSolver> solver(detached_model);
    solver.solve();
}
