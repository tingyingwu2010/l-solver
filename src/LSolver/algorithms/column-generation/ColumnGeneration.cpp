//
// Created by hlefebvr on 19/11/19.
//

#include "Column.h"
#include "ColumnIterator.h"
#include "ColumnGeneration.h"


template<class ExternalSolver>
L::ColumnGeneration<ExternalSolver>::ColumnGeneration(Environment& env, L::Model &restricted_master_problem, L::ColumnIterator &column_iterator)
    : _env(env),
    _column_iterator(column_iterator),
    _restricted_master_problem(restricted_master_problem),
    _restricted_master_problem_solver(*new DirectLPSolver<ExternalSolver>(_restricted_master_problem)) {
}

template<class ExternalSolver>
L::ColumnGeneration<ExternalSolver>::~ColumnGeneration() {
    delete &_restricted_master_problem_solver;
}

template<class ExternalSolver>
void L::ColumnGeneration<ExternalSolver>::actually_solve_hook() {

    Column col = _column_iterator.get_next_column();

    while ( !_column_iterator.is_done() ) {

        if (col.empty()) {
            _restricted_master_problem.objective().status(Infeasible);
            break;
        }

        if (col.reduced_cost() < 0)
            add_column(col);

        _restricted_master_problem_solver.solve();
        if (_restricted_master_problem.objective().status() == Unbounded) {
            _restricted_master_problem.objective().value(std::numeric_limits<float>::lowest());
            break;
        }

        _L_LOG_(Release) << "RMP was solved in " << _restricted_master_problem_solver.last_execution_time()
            << " and ended with status " << _restricted_master_problem.objective().status() << ", associated UB = "
            << _restricted_master_problem.objective().value() << std::endl;

        col = _column_iterator.get_next_column();
    }

    if (_restricted_master_problem.objective().status() == Optimal) {
        _restricted_master_problem_solver.solve();
    }
}

template<class ExternalSolver>
void L::ColumnGeneration<ExternalSolver>::add_column(const L::Column &column) {
    Variable alpha = Variable(_env, "_alpha_" + std::to_string(_generated_columns.size()));
    _restricted_master_problem.add(alpha);
    _restricted_master_problem_solver.add_variable(alpha);
    for (auto coefficient : column.as_constraints()) {
        coefficient.first.expression() += coefficient.second * alpha;
        _restricted_master_problem_solver.rebuild_constraint(coefficient.first);
    }
    _restricted_master_problem.objective().expression() += column.objective_cost() * alpha;
    _restricted_master_problem_solver.rebuild_objective();
    _generated_columns.emplace_back(std::pair<Variable, Column>({ alpha, column }));
}

template<class ExternalSolver>
void L::ColumnGeneration<ExternalSolver>::save_results_hook() {
    // nothing to be done here
    // the values should already have been updated by the last RMP solve
}

template<class ExternalSolver>
const typename L::ColumnGeneration<ExternalSolver>::VariableColumnPairs &L::ColumnGeneration<ExternalSolver>::variable_column_paris() const {
    return _generated_columns;
}
