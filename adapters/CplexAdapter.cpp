//
// Created by hlefebvr on 12/11/19.
//

#include "CplexAdapter.h"

IloNumVar::Type L::CplexAdapter::to_cplex(L::AbstractVariable::Type type) {
    switch (type) {
        case AbstractVariable::Positive: return IloNumVar::Type::Float;
        case AbstractVariable::Negative: return IloNumVar::Type::Float;
        case AbstractVariable::Free: return IloNumVar::Type::Float;
        case AbstractVariable::Binary: return IloNumVar::Type::Bool;
        case AbstractVariable::Integer: return IloNumVar::Type::Int;
        default: throw Exception("Unkown LBDS type");
    }
}

L::CplexAdapter::CplexAdapter() {
    _cplex->setOut(_env->getNullStream());
}


L::CplexAdapter::~CplexAdapter() {
    delete _objective;
    for (auto& m : _variables) {
        delete m.second.first;
        delete m.second.second;
    }
    for (auto& m : _constraints) {
        delete m.second.first;
        delete m.second.second;
    }
    delete _env;
}

void L::CplexAdapter::create_variable(const L::Variable &variable) {
    IloNumVar& cplex_variable = *new IloNumVar(*_env, variable.lb(), variable.ub(), to_cplex(variable.type()), variable.user_defined_name().c_str());
    _variables.insert({ variable.user_defined_name(), { new Variable(variable), &cplex_variable }});
    _model->add(cplex_variable);
}

void L::CplexAdapter::create_constraint(const L::Constraint &constraint) {

    IloNumExpr cplex_expr = IloNumExpr(*_env);
    lbds_expression_to_cplex(constraint.expression(), cplex_expr);

    IloRange* cplex_constraint;
    switch (constraint.type()) {
        case AbstractConstraint::LessOrEqualTo:
            cplex_constraint = new IloRange(cplex_expr <= 0);
            break;
        case AbstractConstraint::GreaterOrEqualTo:
            cplex_constraint = new IloRange(cplex_expr >= 0);
            break;
        case AbstractConstraint::EqualTo:
            cplex_constraint = new IloRange(cplex_expr == 0);
            break;
        default: throw Exception("Unknown constraint type");
    }

    cplex_constraint->setName(constraint.user_defined_name().c_str());
    _constraints.insert({ constraint.user_defined_name(), { new Constraint(constraint), cplex_constraint }});
    _model->add(*cplex_constraint);
}

void L::CplexAdapter::create_objective(const L::Objective &objective) {
    _lbds_objective = new Objective(objective);
    IloNumExpr cplex_expr = IloNumExpr(*_env);
    lbds_expression_to_cplex(objective.expression(), cplex_expr);
    _objective = new IloObjective(*_env, cplex_expr, to_cplex(objective.type()));
    _model->add(*_objective);
}

void L::CplexAdapter::export_to_file(const std::string &filename) {
    _cplex->exportModel(filename.c_str());
}

void L::CplexAdapter::solve() {

    if (_objective == nullptr) throw Exception("Cannot solve model without objective");

    _cplex->solve();

    IloAlgorithm::Status status = _cplex->getStatus();
    _lbds_objective->status(to_lbds(status));
    if (status == IloAlgorithm::Optimal || status == IloAlgorithm::Feasible)
        _lbds_objective->value(_cplex->getObjValue());

}

void L::CplexAdapter::lbds_expression_to_cplex(const L::Expression &lbds_expr, IloNumExpr &cplex_expr) {
    // TODO, this function could be optimized for linear constraints by expand and linear_factorize
    // For the generalize case, expanding the expressions would lead to faster execution of this method

    IloEnv env = cplex_expr.getEnv();

    std::function<void(const Expression&, IloNumExpr&)> traversal;
    traversal = [&traversal, &env, this](const Expression& current, IloNumExpr& result) {
        ExpressionType type = current.type();
        if (type == Num) {
            result += current.as_numerical();
        } else if (type == Var) {
            ConstVariable var = current.as_variable();
            auto found = _variables.find(var.user_defined_name());
            if (found == _variables.end()) throw Exception("Reference to undeclared variable");
            result = *found->second.second;
        } else if(type == Sum || type == Prod) {
            IloNumExpr right = IloNumExpr(env);
            IloNumExpr left = IloNumExpr(env);
            traversal(current.child(Right), right);
            traversal(current.child(Left), left);
            if (type == Sum) result = (left += right);
            else result = (left * right);
        } else {
            throw Exception("Encountered nonlinear operator while building CPLEX model");
        }
    };

    traversal(lbds_expr, cplex_expr);
}

IloObjective::Sense L::CplexAdapter::to_cplex(L::ObjectiveType type) {
    return type == Minimize ? IloObjective::Sense::Minimize : IloObjective::Sense::Maximize;
}

L::ObjectiveStatus L::CplexAdapter::to_lbds(IloAlgorithm::Status status) {
    switch (status) {
        case IloAlgorithm::Unknown: return Error;
        case IloAlgorithm::Feasible: return Feasible;
        case IloAlgorithm::Optimal: return Optimal;
        case IloAlgorithm::Infeasible: return Infeasible;
        case IloAlgorithm::Unbounded: return Unbounded;
        case IloAlgorithm::InfeasibleOrUnbounded: throw Exception("don't know what to do");
        case IloAlgorithm::Error: return Error;
        default: throw Exception("Unknown CPLEX status");
    }
}

void L::CplexAdapter::save_more_results(bool primal, bool dual, bool reduced_costs, bool slacks) {
    if (primal || reduced_costs)
        for (auto& m : _variables) {
            if (primal) m.second.first->value(_cplex->getValue(*m.second.second));
            if (reduced_costs) m.second.first->reduced_cost(_cplex->getReducedCost(*m.second.second));
        }

    if (dual || slacks)
        for (auto& m : _constraints) {
            if (dual) m.second.first->dual().value(_cplex->getDual(*m.second.second));
            if (slacks) m.second.first->slack(_cplex->getSlack(*m.second.second));
        }
}

void L::CplexAdapter::save_results(bool primal, bool dual) {
    if (primal)
        for (auto& m : _variables) {
            m.second.first->value(_cplex->getValue(*m.second.second));
        }

    if (dual)
        for (auto& m : _constraints)
            m.second.first->dual().value(_cplex->getDual(*m.second.second));
}

