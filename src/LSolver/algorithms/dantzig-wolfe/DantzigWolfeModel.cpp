//
// Created by hlefebvr on 22/11/19.
//

#include <LSolver/application/Application.h>
#include "DantzigWolfeModel.h"
#include "../../modeling/constraints/DetachedConstraint.h"

L::DantzigWolfeModel::DantzigWolfeModel(L::Decomposition &decomposition)
    : _decomposition(decomposition) {
    _restricted_master_problem.add(Objective(_env, "_dw_objective"));
    dispatch();
}

L::DantzigWolfeModel::~DantzigWolfeModel() {
    for (auto& m : _sub_problems) delete m.second;
    for (auto ptr : _detached_linking_constraints) delete ptr;
}

void L::DantzigWolfeModel::dispatch() {
    for (Variable var : _decomposition.source_model().variables()) dispatch(var);
    dispatch(_decomposition.source_model().objective());
    for (Constraint ctr : _decomposition.source_model().constraints()) dispatch(ctr);
    add_convex_constraints();
}

void L::DantzigWolfeModel::dispatch(L::Variable &variable) {
    bool has_been_dispatch = false;
    for (const auto& m : _decomposition.block_indicators()) {
        if (m.second(variable)) {
            DantzigWolfeSubProblem* sub_problem;
            auto found = _sub_problems.find(m.first);
            if (found == _sub_problems.end()) {
                sub_problem = new DantzigWolfeSubProblem(m.first);
                _sub_problems.insert({ m.first, sub_problem });
            } else {
                sub_problem = found->second;
            }
            sub_problem->add(variable);
            has_been_dispatch = true;
        }
    }

    if (!has_been_dispatch) _restricted_master_problem.add(variable);
}

void L::DantzigWolfeModel::dispatch(L::Constraint &ctr) {
    auto split = ctr.expression().split_by_variable(_decomposition.block_indicators());

    std::string block_name;
    if (is_linking_constraint(split, block_name)) {
        auto& detached_ctr = *new DetachedConstraint(ctr, false);
        Constraint hidden_detached(detached_ctr);
        detached_ctr.expression() = 0;
        for (const auto &m : split) {
            if (m.first == "_default") detached_ctr.expression() = m.second;
            else {
                DantzigWolfeSubProblem& sub_problem = *_sub_problems.find(m.first)->second;
                sub_problem.add_linking_expression(hidden_detached, m.second);
                sub_problem.objective().expression() -= ctr.dual() * m.second;
            }
        }
        add_artificial_variables(hidden_detached);
        _restricted_master_problem.add(hidden_detached);
        _detached_linking_constraints.emplace_back(&detached_ctr);
    } else {
        if (block_name == "_default") _restricted_master_problem.add(ctr);
        else _sub_problems.find(block_name)->second->add(ctr);
    }
}

bool L::DantzigWolfeModel::is_linking_constraint(const std::map<std::string, Expression> & split, std::string& block_name) {

    // TODO this can be removed as soon as expand() is written and simplifies the expressions !
    auto is_numerical = [](const Expression& expr){ // checks if an expression is an expression
        bool is_numerical = true;
        expr.depth_first_traversal<PostOrder>([&is_numerical](const Expression& node){
            if (node.type() == Var) {
                is_numerical = false;
                throw StopIteration();
            }
        });
        return is_numerical;
    };

    if (split.size() == 1) { // block defining
        block_name = split.begin()->first;
        return false;
    }

    if (split.size() == 2) { // can be block defining
        auto _default = split.end();
        for (auto it = split.begin(), end = split.end() ; it != end ; ++it ) {
            if (it->first == "_default") _default = it;
            else block_name = it->first;
        }

        if (_default != split.end() && is_numerical(_default->second)) { // block defining
            return false;
        }
    }

    return true;
}

void L::DantzigWolfeModel::dispatch(L::Objective obj) {
    auto split = obj.expression().split_by_variable(_decomposition.block_indicators());

    for (const auto& m : split) {
        if (m.first == "_default") {
            _restricted_master_problem.objective().expression() += m.second;
        } else {
            DantzigWolfeSubProblem& sub_problem = *_sub_problems.find(m.first)->second;
            sub_problem.objective().expression() += m.second;
            sub_problem.set_linking_cost(m.second);
        }
    }

}

void L::DantzigWolfeModel::add_artificial_variables(Constraint& ctr) {
    const float artificial_cost = _restricted_master_problem.objective().ub();

    auto artificial_variable = [this, &artificial_cost](){
        Variable a = Variable(_env, "_artificial_" + std::to_string(_n_artificial_var++));
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

void L::DantzigWolfeModel::add_convex_constraints() {
    for (const auto& m : _sub_problems) {
        Constraint convex = Constraint(_env, "_convex_" + m.first);
        convex.expression() += -1;
        convex.type(EqualTo);
        add_artificial_variables(convex);
        _restricted_master_problem.add(convex);
        m.second->set_convex_constraint(convex);
    }
}

L::Model &L::DantzigWolfeModel::restricted_master_problem() {
    return _restricted_master_problem;
}

L::Environment &L::DantzigWolfeModel::env() {
    return _env;
}







L::DantzigWolfeSubProblem::~DantzigWolfeSubProblem() {
    delete _convex_constraint;
}

const std::map<std::string, L::DantzigWolfeSubProblem *> &L::DantzigWolfeModel::sub_problems() {
    return _sub_problems;
}

L::DantzigWolfeSubProblem::DantzigWolfeSubProblem(const std::string &user_defind_name)
    : Model(user_defind_name),
      _objective(Objective(_env, "detached_objective_" + user_defind_name))
{
    add(_objective);
}

void L::DantzigWolfeSubProblem::add_linking_expression(const L::Constraint &ctr, const L::Expression &expr) {
    _linking_constraints.emplace_back(std::pair<Constraint, Expression>({ ctr, expr }));
}

void L::DantzigWolfeSubProblem::set_linking_cost(const L::Expression &expr) {
    _linking_cost = expr;
}

const L::DantzigWolfeSubProblem::LinkingConstraintExpression &
L::DantzigWolfeSubProblem::linking_constraint_expressions() const {
    return _linking_constraints;
}

void L::DantzigWolfeSubProblem::set_convex_constraint(const L::Constraint &ctr) {
    delete _convex_constraint;
    _convex_constraint = new Constraint(ctr);
}

L::Constraint L::DantzigWolfeSubProblem::convex_constraint() {
    return *_convex_constraint;
}

L::Expression &L::DantzigWolfeSubProblem::linking_cost() {
    return _linking_cost;
}
