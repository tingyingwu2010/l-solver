//
// Created by hlefebvr on 14/11/19.
//

#include "../../application/LogManager.h"

template <class ExternalSolver>
L::DirectLPSolver<ExternalSolver>::DirectLPSolver(L::Model &model) : DirectSolver(model) {
    build_lp_model();
}

template <class ExternalSolver>
L::DirectLPSolver<ExternalSolver>::~DirectLPSolver() {
    for (auto& m : _detached_variables) delete m;
}

template <class ExternalSolver>
void L::DirectLPSolver<ExternalSolver>::actually_solve() {
    _solver.solve();
}

template<class ExternalSolver>
void L::DirectLPSolver<ExternalSolver>::save_results() {
    ObjectiveStatus status = _model.objective().status();
    if (status == Optimal || status == Feasible) {
        _solver.save_results(true, false);
        for (auto& m : _detached_variables)
            m->update_core_value();
    }
}

template<class ExternalSolver>
void L::DirectLPSolver<ExternalSolver>::build_lp_model() {
    for (auto variable : _model.variables()) {
        if (variable.type() == Binary || variable.type() == Integer) {
            _L_LOG_(Debug) << "Because variable " << variable.user_defined_name() << " has to be relaxed to be solved with an LP solver, a detached variable is created.\n";
            DetachedVariable* x = new DetachedVariable(variable);
            x->type(Free);
            _detached_variables.emplace_back(x);
            _solver.create_variable(Variable(*x));
        } else {
            _solver.create_variable(variable);
        }
    }
    for (auto constraints : _model.constraints()) _solver.create_constraint(constraints);
    _solver.create_objective(_model.objective());
}
