//
// Created by hlefebvr on 14/11/19.
//

#include "../../application/LogManager.h"
#include "DirectLPSolver.h"


template <class ExternalSolver>
L::DirectLPSolver<ExternalSolver>::DirectLPSolver(L::Model &model) : DirectSolver(model) {
    build_lp_model();
}

template <class ExternalSolver>
L::DirectLPSolver<ExternalSolver>::~DirectLPSolver() {
    for (auto& m : _detached_variables) delete m;
}

template <class ExternalSolver>
void L::DirectLPSolver<ExternalSolver>::actually_solve_hook() {
    _solver.solve();
}

template<class ExternalSolver>
void L::DirectLPSolver<ExternalSolver>::save_results_hook() {
    ObjectiveStatus status = _model.objective().status();
    if (status != Infeasible && status != Error) {
        _solver.save_results(true, true);
        for (auto& m : _detached_variables)
            m->update_core_value();
    }
    if (status == Unbounded) {
        _model.objective().value(std::numeric_limits<float>::lowest());
    }
}

template<class ExternalSolver>
void L::DirectLPSolver<ExternalSolver>::build_lp_model() {
    for (auto variable : _model.variables()) {
        if (variable.type() == Binary) {
            DetachedVariable* x = new DetachedVariable(variable);
            x->type(Positive);
            x->ub(variable.ub());
            x->lb(variable.lb());
            _detached_variables.emplace_back(x);
            _solver.create_variable(Variable(*x));
        } else if (variable.type() == Integer) {
            DetachedVariable* x = new DetachedVariable(variable);
            x->type(Positive);
            _detached_variables.emplace_back(x);
            _solver.create_variable(Variable(*x));
        } else {
            _solver.create_variable(variable);
        }
    }
    for (auto constraints : _model.constraints()) _solver.create_constraint(constraints);
    _solver.create_objective(_model.objective());
}

template<class ExternalSolver>
void L::DirectLPSolver<ExternalSolver>::export_to_file(const std::string& filename) {
    _solver.export_to_file(filename);
}

template<class ExternalSolver>
void L::DirectLPSolver<ExternalSolver>::rebuild_objective() {
    _solver.rebuild_objective();
}

template<class ExternalSolver>
void L::DirectLPSolver<ExternalSolver>::add_variable(const L::Variable &variable) {
    _solver.create_variable(variable);
}

template<class ExternalSolver>
void L::DirectLPSolver<ExternalSolver>::add_constraint(const L::Constraint &constraint) {
    _solver.create_constraint(constraint);
}

template<class ExternalSolver>
void L::DirectLPSolver<ExternalSolver>::rebuild_constraint(const L::Constraint &ctr) {
    _solver.rebuild_constraint(ctr);
}

