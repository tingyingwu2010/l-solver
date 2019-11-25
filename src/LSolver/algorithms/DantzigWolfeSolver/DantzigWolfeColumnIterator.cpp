//
// Created by hlefebvr on 22/11/19.
//

#include <LSolver/application/LogManager.h>
#include "DantzigWolfeModel.h"


template<class SubProblemSolver>
L::DantzigWolfeColumnIterator<SubProblemSolver>::DantzigWolfeColumnIterator(
        const std::map<std::string, DantzigWolfeSubProblem *> & sub_problems) {
    for (auto& m : sub_problems)
        _sub_problems.insert({ m.second, new SubProblemSolver(*m.second) });
    _sub_problems_iterator = _sub_problems.end();
}

template<class SubProblemSolver>
L::Column L::DantzigWolfeColumnIterator<SubProblemSolver>::get_next_column() {
    next_sub_problem();

    DantzigWolfeSubProblem& sub_problem_model = *_sub_problems_iterator->first;
    SubProblemSolver& sub_problem_solver = *_sub_problems_iterator->second;
    Column output;
    output.reduced_cost(std::numeric_limits<float>::lowest());

    sub_problem_solver.rebuild_objective();
    sub_problem_solver.solve();

    switch (sub_problem_model.objective().status()) {
        case Infeasible: return output;
        case Feasible:
        case Optimal: {
            output.reduced_cost( sub_problem_model.objective().value() - sub_problem_model.convex_constraint().dual().value() );
            output.constraint_coefficient(sub_problem_model.convex_constraint(), 1);
            [[fallthrough]];
        }
        case Unbounded: {
            for (const auto& m : sub_problem_model.linking_constraint_expressions()) {
                Expression expr = m.second;
                output.constraint_coefficient(m.first, expr.feval());
            }
            for (const Variable& variable : sub_problem_model.variables())
                output.variable_coefficient(variable, variable.value());
            output.objective_cost(sub_problem_model.linking_cost().feval());
            break;
        }
        default: throw Exception("Subproblem " + sub_problem_model.user_defined_name() + " ended with undesirable status");
    }

    _L_LOG_(Release) << "Pricing problem was solved in " << sub_problem_solver.last_execution_time()
                     << ", and ended with status " << sub_problem_model.objective().status()
                     << " a column was generated with associated reduced cost = " << output.reduced_cost() << std::endl;

    if (output.reduced_cost() < 0 - Application::parameters().tolerance())
        _last_iteration_improved = true;

    return output;
}

template<class SubProblemSolver>
bool L::DantzigWolfeColumnIterator<SubProblemSolver>::is_done() {
    return _is_done;
}

template<class SubProblemSolver>
L::DantzigWolfeColumnIterator<SubProblemSolver>::DantzigWolfeColumnIterator(
        const L::DantzigWolfeColumnIterator<SubProblemSolver> &rhs) : ColumnIterator(rhs) {}

template<class SubProblemSolver>
void L::DantzigWolfeColumnIterator<SubProblemSolver>::next_sub_problem() {
    if (_sub_problems_iterator == _sub_problems.end()) {
        _sub_problems_iterator = _sub_problems.begin();
    } else {
        ++_sub_problems_iterator;
        if (_sub_problems_iterator == _sub_problems.end()) {
            if (!_last_iteration_improved) _is_done = true;
            _last_iteration_improved = false;
            _sub_problems_iterator = _sub_problems.begin();
        }
    }
}

template<class SubProblemSolver>
void L::DantzigWolfeColumnIterator<SubProblemSolver>::reset_sub_problems_variables() {
    for (auto& m : _sub_problems)
        for (Variable var : m.first->variables())
            var.value(0);
}
