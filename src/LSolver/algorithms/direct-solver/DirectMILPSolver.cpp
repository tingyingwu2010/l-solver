//
// Created by hlefebvr on 14/11/19.
//

#include "DirectMILPSolver.h"

template <class ExternalSolver>
L::DirectMILPSolver<ExternalSolver>::DirectMILPSolver(L::Model &model) : DirectSolver(model) {
    build_milp_model();
}

template <class ExternalSolver>
void L::DirectMILPSolver<ExternalSolver>::actually_solve_hook() {
    _solver.solve();
}

template<class ExternalSolver>
void L::DirectMILPSolver<ExternalSolver>::save_results_hook() {
    ObjectiveStatus status = _model.objective().status();
    if (status == Optimal || status == Feasible)
        _solver.save_results(true, false);
    if (status == Unbounded)
        _model.objective().value(std::numeric_limits<float>::lowest());
}

template<class ExternalSolver>
void L::DirectMILPSolver<ExternalSolver>::build_milp_model() {
    for (auto variable : _model.variables()) _solver.create_variable(variable);
    for (auto constraints : _model.constraints()) _solver.create_constraint(constraints);
    _solver.create_objective(_model.objective());
}

template<class ExternalSolver>
void L::DirectMILPSolver<ExternalSolver>::rebuild_objective() {
    _solver.rebuild_objective();
}
