//
// Created by hlefebvr on 19/11/19.
//

#include <regex>
#include "../../modeling/constraints/DetachedConstraint.h"
#include "LSolver/modeling/models/Decomposition.h"

template<class ExternalSolver>
L::DantzigWolfeDecomposition<ExternalSolver>::~DantzigWolfeDecomposition() {
    delete _cg_solver;
    delete _dw_column_generator;
}

template<class ExternalSolver>
L::DantzigWolfeDecomposition<ExternalSolver>::DantzigWolfeDecomposition(L::Decomposition &decomposition)
    : _model(DualAngularModel(decomposition)),
      _rmp_as_block(_model.block("_default")),
      _restricted_master_problem(DetachedModel(_rmp_as_block)),
      _decomposition(decomposition)
{
    build_restricted_master_problem();
    _dw_column_generator = new DantzigWolfeColumnIterator<ExternalSolver>(_restricted_master_problem, _model);
    _cg_solver = new ColumnGeneration<ExternalSolver>(_dw_env, _restricted_master_problem, *_dw_column_generator);
}

template<class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::add_artificial_variable(L::Constraint &ctr) {
    const float artificial_cost = 100;

    auto artificial_variable = [this, &artificial_cost](){
        Variable a = Variable(_dw_env, "_artificial_" + std::to_string(_n_artificial_var++));
        _restricted_master_problem.objective().expression() += artificial_cost * a;
        _restricted_master_problem.add(a);
        return a;
    };

    switch (ctr.type()) {
        case LessOrEqualTo: ctr.expression() -= artificial_variable(); break;
        case GreaterOrEqualTo: ctr.expression() += artificial_variable(); break;
        case EqualTo: ctr.expression() += artificial_variable() - artificial_variable(); break;
        default: throw Exception("Unknown constraints type: " + std::to_string(ctr.type()));
    }
}

template<class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::build_restricted_master_problem() {
    // add linking constraints and artificial variables
    for (LinkingConstraint& ctr : _model.linking_constraints()) {
        DetachedConstraint& restricted_linking_constraint = *new DetachedConstraint(ctr, false);
        restricted_linking_constraint.expression() = ctr.block("_default");
        Constraint casted = Constraint(restricted_linking_constraint);
        add_artificial_variable(casted);
        _restricted_master_problem.add(restricted_linking_constraint);
    }

    // add convex_constraints
    for (Model& block: _model.blocks()) {
        if (block.user_defined_name() == "_default") continue;
        Constraint convex_constraint = Constraint(_dw_env, "_convex_" + block.user_defined_name());
        convex_constraint.type(EqualTo);
        convex_constraint.expression() += -1;
        add_artificial_variable(convex_constraint);
        _restricted_master_problem.add(convex_constraint);
    }

}

template<class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::actually_solve() {
    if (!_cg_solver) throw Exception("An error occured.");
    _cg_solver->solve();
}

template<class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::save_results() {
    ObjectiveStatus status =_restricted_master_problem.objective().status();
    if(status == Optimal || status == Feasible) {
        for (const Variable& var : _restricted_master_problem.variables()) {
            if (std::regex_match(var.user_defined_name(), std::regex("_artificial_([0-9]+)"))
                && var.value() > 0 + Application::parameters().tolerance()) {
                status = Infeasible;
                break;
            }
        }

        // build original variables
        for (Variable variable : _decomposition.source_model().variables()) variable.value(0);
        for (const auto& p : _cg_solver->variable_column_paris()) {
            Variable alpha = p.first;
            for (const auto& var_coef : p.second.as_variables()) {
                float v = _decomposition.source_model().variable(var_coef.first).value();
                _decomposition.source_model().variable(var_coef.first).value(v + alpha.value() * var_coef.second );
            }
        }
    }
    _decomposition.source_model().objective().status(status);
    _decomposition.source_model().objective().value(_restricted_master_problem.objective().value());
}
