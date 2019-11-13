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

L::CplexAdapter::~CplexAdapter() {
    delete _objective;
    for (auto& m : _variables) delete m.second.second;
    for (auto& m : _constraints) delete m.second.second;
    delete _env;
}

void L::CplexAdapter::create_variable(const L::Variable &variable) {
    IloNumVar& cplex_variable = *new IloNumVar(*_env, variable.lb(), variable.ub(), to_cplex(variable.type()), variable.user_defined_name().c_str());
    _variables.insert({ variable.user_defined_name(), { Variable(variable), &cplex_variable }});
    _model->add(cplex_variable);
}

void L::CplexAdapter::create_constraint(const L::Constraint &constraint) {
    IloNumExpr cplex_expr = IloNumExpr(*_env);
    lbds_expression_to_cplex(constraint.expression(), cplex_expr);
    IloRange& cplex_constraint = *new IloRange(*_env, cplex_expr, 0, constraint.user_defined_name().c_str());
    _constraints.insert({ constraint.user_defined_name(), { Constraint(constraint), &cplex_constraint }});
    _model->add(cplex_constraint);
}

void L::CplexAdapter::export_to_file(const std::string &filename) {
    _cplex->exportModel(filename.c_str());
}

void L::CplexAdapter::solve() {
    _cplex->solve();
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
