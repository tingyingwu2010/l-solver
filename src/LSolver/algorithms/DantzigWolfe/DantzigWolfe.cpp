//
// Created by hlefebvr on 19/11/19.
//

#include <regex>
#include "DantzigWolfe.h"

template<class ExternalSolver>
L::DantzigWolfe<ExternalSolver>::~DantzigWolfe() {
    delete _cg_solver;
    delete _dw_column_generator;
}

template<class ExternalSolver>
L::DantzigWolfe<ExternalSolver>::DantzigWolfe(L::DualAngularModel &model)
    : _model(model),
      _rmp_as_block(_model.block("others")),
      _restricted_master_problem(DetachedModel(_rmp_as_block))
{
    build_restricted_master_problem();
    _dw_column_generator = new DantzigWolfeColumnIterator<ExternalSolver>(_restricted_master_problem, _model);
    _cg_solver = new ColumnGeneration<ExternalSolver>(_restricted_master_problem, *_dw_column_generator);
}

template<class ExternalSolver>
void L::DantzigWolfe<ExternalSolver>::add_artificial_variable(L::Constraint &ctr) {
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
        default: throw Exception("Unknown constraint type: " + std::to_string(ctr.type()));
    }
}

template<class ExternalSolver>
void L::DantzigWolfe<ExternalSolver>::build_restricted_master_problem() {
    // add linking constraints and artificial variables
    for (LinkingConstraint& ctr : _model.linking_constraints()) {
        DetachedConstraint& restricted_linking_constraint = *new DetachedConstraint(ctr, false);
        restricted_linking_constraint.expression() = ctr.block("others");
        Constraint casted = Constraint(restricted_linking_constraint);
        add_artificial_variable(casted);
        _restricted_master_problem.add(Constraint(restricted_linking_constraint));
    }

    // add convex_constraints
    for (Model& block: _model.blocks()) {
        if (block.user_defined_name() == "others") continue;
        Constraint convex_constraint = Constraint(_dw_env, "_convex_" + block.user_defined_name());
        convex_constraint.type(EqualTo);
        convex_constraint.expression() += -1;
        add_artificial_variable(convex_constraint);
        _restricted_master_problem.add(convex_constraint);
    }

}

template<class ExternalSolver>
void L::DantzigWolfe<ExternalSolver>::actually_solve() {
    if (!_cg_solver) throw Exception("An error occured.");
    _cg_solver->solve();
}

template<class ExternalSolver>
void L::DantzigWolfe<ExternalSolver>::save_results() {
    ObjectiveStatus status =_restricted_master_problem.objective().status();
    if(status == Optimal) {
        for (const Variable& var : _restricted_master_problem.variables()) {
            if (std::regex_match(var.user_defined_name(), std::regex("_artificial_([0-9]+)"))
                && var.value() > 0 + Application::parameters().tolerance()) {
                status = Infeasible;
                break;
            }
        }
    }
    _model.source_model().objective().status(status);
    _model.source_model().objective().value(_restricted_master_problem.objective().value());
}

//// GENERATING COLUMNS

template<class ExternalSolver>
L::DantzigWolfeColumnIterator<ExternalSolver>::DantzigWolfeColumnIterator(L::Model &rmp, DualAngularModel& da_model)
    : _restricted_master_problem(rmp), _dual_angular_model(da_model) {
    build_subproblems();
}

template<class ExternalSolver>
void L::DantzigWolfeColumnIterator<ExternalSolver>::build_subproblems() {
    for (Model& block : _dual_angular_model.blocks()) {
        if (block.user_defined_name() == "others") continue;
        _L_LOG_(Debug) << "Building subproblem's model: " << block.user_defined_name() << std::endl;
        DetachedModel& subproblem_model = *new DetachedModel(block);
        for (LinkingConstraint& ctr : _dual_angular_model.linking_constraints()) {
            try {
                subproblem_model.objective().expression() -= ctr.dual() * ctr.block(block.user_defined_name());
            } catch (const NotFound&) {}
        }
        DirectLPSolver<ExternalSolver>& subproblem_solver = *new DirectLPSolver<ExternalSolver>(subproblem_model);
        _subproblems.emplace_back(ModelSolverAssoc(&subproblem_model, &subproblem_solver));
    }
    _subproblem_iterator = _subproblems.end();
}

template<class ExternalSolver>
L::Column L::DantzigWolfeColumnIterator<ExternalSolver>::get_next_column() {
    next_subproblem();

    DetachedModel& subproblem_model = *_subproblem_iterator->first;
    DirectLPSolver<ExternalSolver>& subproblem_solver = *_subproblem_iterator->second;
    const std::string& block_name = subproblem_model.user_defined_name();
    Column output;

    subproblem_solver.rebuild_objective();
    subproblem_solver.solve();

    float reduced_cost = std::numeric_limits<float>::lowest();
    subproblem_model.update_primal_values();

    // build column
    for (LinkingConstraint &ctr : _dual_angular_model.linking_constraints())
        output.coefficient(ctr.user_defined_name(), ctr.block(block_name).feval());
    output.objective_cost(_dual_angular_model.block(block_name).objective().expression().feval());

    if (subproblem_model.objective().status() == Optimal) {
        float dual_value_of_convex_combination = _restricted_master_problem.constraint(
                "_convex_" + block_name).dual().value();
        reduced_cost = -dual_value_of_convex_combination + subproblem_model.objective().value();

        // add convex combination coefficient
        output.coefficient("_convex_" + block_name, 1);
    }

    output.reduced_cost(reduced_cost);

    _L_LOG_(Release) << "Pricing problem was solved in " << subproblem_solver.last_execution_time()
                     << ", and ended with status " << subproblem_model.objective().status()
                     << " a column was generated with associated reduced cost = " << output.reduced_cost() << std::endl;

    if (reduced_cost <= 0 - Application::parameters().tolerance()) _last_iteration_improved = true;

    return output;
}

template<class ExternalSolver>
void L::DantzigWolfeColumnIterator<ExternalSolver>::next_subproblem() {
    if (_subproblem_iterator == _subproblems.end()) {
        _subproblem_iterator = _subproblems.begin();
    } else {
        ++_subproblem_iterator;
        if (_subproblem_iterator == _subproblems.end()) {
            if (!_last_iteration_improved) _is_done = true;
            _last_iteration_improved = false;
            _subproblem_iterator = _subproblems.begin();
        }
    }
}

template<class ExternalSolver>
L::DantzigWolfeColumnIterator<ExternalSolver>::~DantzigWolfeColumnIterator() {}

template<class ExternalSolver>
bool L::DantzigWolfeColumnIterator<ExternalSolver>::is_done() {
    return _is_done;
}
