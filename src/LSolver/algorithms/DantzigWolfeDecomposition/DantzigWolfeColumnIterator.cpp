//
// Created by hlefebvr on 20/11/19.
//

template<class ExternalSolver>
L::DantzigWolfeColumnIterator<ExternalSolver>::DantzigWolfeColumnIterator(L::Model &rmp, DualAngularModel& da_model)
        : _restricted_master_problem(rmp), _dual_angular_model(da_model) {
    build_subproblems();
}

template<class ExternalSolver>
void L::DantzigWolfeColumnIterator<ExternalSolver>::build_subproblems() {
    for (Model& block : _dual_angular_model.blocks()) {
        if (block.user_defined_name() == "_default") continue;
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
    subproblem_model.update_primal_values();

    float reduced_cost = std::numeric_limits<float>::lowest();

    switch (subproblem_model.objective().status()) {
        case Infeasible: return output;
        case Feasible:
        case Optimal: {
            float dual_value_of_convex_combination = _restricted_master_problem.constraint("_convex_" + block_name).dual().value();
            reduced_cost = -dual_value_of_convex_combination + subproblem_model.objective().value();
            output.coefficient("_convex_" + block_name, 1);
            [[fallthrough]];
            }
        case Unbounded: {
            for (LinkingConstraint &ctr : _dual_angular_model.linking_constraints())
                output.coefficient(ctr.user_defined_name(), ctr.block(block_name).feval());
            output.objective_cost(_dual_angular_model.block(block_name).objective().expression().feval());
            break;
        }
        default: throw Exception("Subproblem " + subproblem_model.user_defined_name() + " ended with indesirable status");
    }

    output.reduced_cost(reduced_cost);

    _L_LOG_(Release) << "Pricing problem was solved in " << subproblem_solver.last_execution_time()
                     << ", and ended with status " << subproblem_model.objective().status()
                     << " a column was generated with associated reduced cost = " << output.reduced_cost() << std::endl;

    if (reduced_cost < 0 - Application::parameters().tolerance()) _last_iteration_improved = true;

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
