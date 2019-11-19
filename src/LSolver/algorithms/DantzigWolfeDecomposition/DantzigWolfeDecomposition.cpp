//
// Created by hlefebvr on 16/11/19.
//

#include "../../modeling/DualAngularModel.h"
#include "DantzigWolfeDecomposition.h"


template<class ExternalSolver>
L::DantzigWolfeDecomposition<ExternalSolver>::DantzigWolfeDecomposition(L::Model &model) : _model(model) {}

template <class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::actually_solve() {
    if (!_rmp_solver) throw Exception("RMP solver has not been initialized. Please, call the decompose() method after having passed your decomposition indicators.");
    Model& rmp = _decomposed_model->block("others");

    // initial columns
    for (auto& m : _subproblem_solvers) {
        Model& block = _decomposed_model->block(m.first);
        m.second->solve();
        _L_LOG_(Release) << "Pricing problem with dual = 0 to initiate columns  " << m.first << " was solved in " << m.second->last_execution_time()
                         << " and has status " << block.objective().status()
                         << ", reduced cost = " << block.objective().value() << std::endl;
        if (block.objective().status() == Optimal) add_extreme_point(block);
        else add_extreme_ray(block);
    }

    unsigned int i = 0;
    do {
        _rmp_solver->solve();
        _L_LOG_(Release) << "RMP was solved in " << _rmp_solver->last_execution_time() << " and has status "
            << rmp.objective().status() << ", UB = " << rmp.objective().value() << std::endl;
        if (i++ == 4) break;
        for (auto& m : _subproblem_solvers) {
            Model& block = _decomposed_model->block(m.first);
            m.second->rebuild_objective();
            m.second->solve();
            _L_LOG_(Release) << "Pricing problem  " << m.first << " was solved in " << m.second->last_execution_time()
                << " and has status " << block.objective().status()
                << ", reduced cost = " << block.objective().value() << std::endl;
            if (block.objective().status() == Optimal) add_extreme_point(block);
            else add_extreme_ray(block);
        }
    } while (true);
}

template <class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::save_results() {

}

template<class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::add_decomposition(const std::string& name, const L::VariableIndicator& indicator) {
    _indicators.insert({ name, indicator });
}

template<class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::decompose() {
    _L_LOG_(Debug) << "Decomposing given model." << std::endl;
    _decomposed_model = new DualAngularModel(_model, _indicators);
    _L_LOG_(Debug) << *_decomposed_model << std::endl;
    build_problems();
}

template<class ExternalSolver>
L::DantzigWolfeDecomposition<ExternalSolver>::~DantzigWolfeDecomposition() {
    delete _decomposed_model;
    delete _rmp_solver;
    for (auto& m : _subproblem_solvers) delete m.second;
    for (auto& m : _detached_constraints) delete m.second;
}

template<class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::build_problems() {
    for (Model& block : _decomposed_model->blocks()) {
        if (block.user_defined_name() == "others") {
            _rmp_model = new Model();
            _rmp_model->add(block.objective()); // copy objective
            for (Constraint ctr : block.constraints()) { // copy block defining constraints
                DetachedConstraint* detached = new DetachedConstraint(ctr, false);
                detached->expression() = ctr.expression();
                detached->type(ctr.type());
                _detached_constraints.insert({detached->user_defined_name(), detached});

                // add artifical variable
                Variable a = Variable(_dw_env, "_artifical_" + ctr.user_defined_name());
                if (ctr.type() == LessOrEqualTo) detached->expression() -= a;
                else if (ctr.type() == GreaterOrEqualTo) detached->expression() += a;
                else throw Exception("Not implemented yet");
                _rmp_model->objective().expression() += 50 * a; // todo, proper bound on objective
                _rmp_model->add(a);

                _rmp_model->add(Constraint(*detached));
            }
            for (LinkingConstraint ctr : _decomposed_model->linking_constraints()) { // copy linking constraints
                DetachedConstraint* detached = new DetachedConstraint(ctr, false);
                detached->expression() = ctr.block(block.user_defined_name());
                detached->type(ctr.type());

                // add artifical variable
                Variable a = Variable(_dw_env, "_artifical_" + ctr.user_defined_name());
                if (ctr.type() == LessOrEqualTo) detached->expression() -= a;
                else if (ctr.type() == GreaterOrEqualTo) detached->expression() += a;
                else throw Exception("Not implemented yet");
                _rmp_model->objective().expression() += 50 * a; // todo, proper bound on objective
                _rmp_model->add(a);

                _detached_constraints.insert({detached->user_defined_name(), detached});
                _rmp_model->add(Constraint (*detached));
            }
            for (Variable variable : block.variables()) _rmp_model->add(variable);
            _rmp_solver = new DirectLPSolver<ExternalSolver>(*_rmp_model);
        } else {
            _cost_functions.insert({ block.user_defined_name(), block.objective().expression() });
            for (LinkingConstraint& ctr : _decomposed_model->linking_constraints()) {
                try {
                    block.objective().expression() -= ctr.dual() * ctr.block(block.user_defined_name());
                } catch (const Exception&) {}
            }
            auto* solver = new DirectLPSolver<ExternalSolver>(block);
            _subproblem_solvers.insert({block.user_defined_name(), solver});
        }
    }
}

template<class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::add_extreme_point(L::Model &block) {
    Variable alpha = Variable(_dw_env, "_alpha_" + std::to_string(_column_counter++));
    _rmp_solver->add_variable(alpha);
    Constraint& convex_ctr = convex_constraint(block.user_defined_name());
    convex_ctr.expression() += alpha;
    _rmp_solver->rebuild_constraint(convex_ctr);

    add_column(alpha, block);
}

template<class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::add_extreme_ray(L::Model& block) {
    throw Exception("Not implemented yet");
}

template<class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::add_column(const L::Variable &variable, L::Model &block) {

    for (LinkingConstraint& ctr : _decomposed_model->linking_constraints()) {
        Expression& expr = ctr.block(block.user_defined_name());
        float coef = expr.feval();

        DetachedConstraint& rmp_ctr = *_detached_constraints.find(ctr.user_defined_name())->second;
        rmp_ctr.expression() += variable * coef;

        _rmp_solver->rebuild_constraint(Constraint(rmp_ctr));
    }

    auto found = _cost_functions.find(block.user_defined_name());
    if (found == _cost_functions.end()) throw Exception("Should this happen ?");

    float coef = found->second.feval();
    _rmp_model->objective().expression() += variable * coef;
    _rmp_solver->rebuild_objective();

    _rmp_solver->export_to_file("rmp.lp");
}

template<class ExternalSolver>
L::Constraint &L::DantzigWolfeDecomposition<ExternalSolver>::convex_constraint(const std::string& block_name) {
    auto found = _convex_constraints.find(block_name);
    if (found == _convex_constraints.end()) {
        Constraint* ctr = new Constraint(_dw_env, "_convex_constraint_" + block_name);
        ctr->type(EqualTo);
        ctr->expression() -= 1;
        _rmp_solver->add_constraint(*ctr);
        _convex_constraints.insert({ block_name, ctr });
        return *ctr;
    }
    return *found->second;
}
