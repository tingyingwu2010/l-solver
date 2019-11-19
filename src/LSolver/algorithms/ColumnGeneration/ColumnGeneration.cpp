//
// Created by hlefebvr on 19/11/19.
//

#include "ColumnGeneration.h"

template<class ExternalSolver>
L::ColumnGeneration<ExternalSolver>::ColumnGeneration(L::Model &restricted_master_problem, L::ColumnIterator &column_iterator)
    : _column_iterator(column_iterator),
    _restricted_master_problem(restricted_master_problem),
    _restricted_master_problem_solver(*new DirectLPSolver<ExternalSolver>(_restricted_master_problem)) {
}

template<class ExternalSolver>
void L::ColumnGeneration<ExternalSolver>::actually_solve() {

    Column col = _column_iterator.get_next_column();
    unsigned int i = 0;
    while ( !_column_iterator.is_done() ) {
        add_column(col);
        _restricted_master_problem_solver.solve();
        _L_LOG_(Release) << "RMP was solved in " << _restricted_master_problem_solver.last_execution_time()
            << " and ended with status " << _restricted_master_problem.objective().status() << ", associated UB = "
            << _restricted_master_problem.objective().value() << std::endl;

        col = _column_iterator.get_next_column();
    }

}

template<class ExternalSolver>
L::ColumnGeneration<ExternalSolver>::~ColumnGeneration() {
    delete &_restricted_master_problem_solver;
}

template<class ExternalSolver>
void L::ColumnGeneration<ExternalSolver>::add_column(const L::Column &column) {
    Variable alpha = Variable(_cg_env, "_alpha_" + std::to_string(_generated_columns.size()));
    _restricted_master_problem.add(alpha);
    _restricted_master_problem_solver.add_variable(alpha);
    for (const auto& coefficient : column.coefficients()) {
        Constraint rmp_ctr = _restricted_master_problem.constraint(coefficient.first);
        rmp_ctr.expression() += coefficient.second * alpha;
        _restricted_master_problem_solver.rebuild_constraint(rmp_ctr);
    }
    _restricted_master_problem.objective().expression() += column.objective_cost() * alpha;
    _restricted_master_problem_solver.rebuild_objective();
    _generated_columns.emplace_back(column);
}

const L::Column::Coefficients &L::Column::coefficients() const {
    return _coefficients;
}

float L::Column::reduced_cost() const {
    return _reduced_cost;
}

void L::Column::reduced_cost(float r) {
    _reduced_cost = r;
}

void L::Column::coefficient(const std::string &ctr, float coef) {
    _coefficients.emplace_back(std::pair<std::string, float>( ctr, coef ));
}

void L::Column::objective_cost(float c) {
    _objective_cost = c;
}

float L::Column::objective_cost() const {
    return _objective_cost;
}
