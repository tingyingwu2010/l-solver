//
// Created by hlefebvr on 14/11/19.
//

#include "DirectMILPSolver.h"

template <class ExternalSolver>
L::DirectMILPSolver<ExternalSolver>::DirectMILPSolver(L::Model &model) : DirectSolver(model) {
    build_milp_model();
}

template <class ExternalSolver>
void L::DirectMILPSolver<ExternalSolver>::actually_solve() {
    _solver.solve();
}

template<class ExternalSolver>
void L::DirectMILPSolver<ExternalSolver>::save_results() {
    ObjectiveStatus status = _model.objective().status();
    if (status == Optimal || status == Feasible)
        _solver.save_results(true, false);
}

template<class ExternalSolver>
void L::DirectMILPSolver<ExternalSolver>::build_milp_model() {
    for (auto variable : _model.variables()) _solver.create_variable(variable);
    for (auto constraints : _model.constraints()) _solver.create_constraint(constraints);
    _solver.create_objective(_model.objective());
}
