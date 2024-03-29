//
// Created by hlefebvr on 12/11/19.
//

#include <iostream>
#include "CplexAdapter.h"
#include "../application/LogManager.h"

IloNumVar::Type L::CplexAdapter::to_cplex(L::VariableType type) {
    switch (type) {
        case Positive: return IloNumVar::Type::Float;
        case Negative: return IloNumVar::Type::Float;
        case Free: return IloNumVar::Type::Float;
        case Binary: return IloNumVar::Type::Bool;
        case Integer: return IloNumVar::Type::Int;
        default: throw Exception("Unkown LBDS type");
    }
}

L::CplexAdapter::CplexAdapter() {
    _env = new IloEnv();
    _model = new IloModel(*_env);
    _cplex = new IloCplex(*_model);
    if (Application::parameters().external_solver_logs()) {
        _logger = new LogManager(External);
        _cplex->setOut(*_logger);
        _cplex->setWarning(*_logger);
    } else {
        _cplex->setOut(_env->getNullStream());
        _cplex->setWarning(_env->getNullStream());
    }
}


L::CplexAdapter::~CplexAdapter() {
    if (_env) _env->end();
    delete _objective;
    delete _lbds_objective;
    delete _model;
    delete _env;
    delete _cplex;
    if(_logger) _logger->flush();
    delete _logger;
    for (auto& m : _variables) {
        delete m.second.first;
        delete m.second.second;
    }
    for (auto& m : _constraints) {
        delete m.second.first;
        delete m.second.second;
    }
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
        case LessOrEqualTo:
            cplex_constraint = new IloRange(cplex_expr <= 0);
            break;
        case GreaterOrEqualTo:
            cplex_constraint = new IloRange(cplex_expr >= 0);
            break;
        case EqualTo:
            cplex_constraint = new IloRange(cplex_expr == 0);
            break;
        default: throw Exception("Unknown constraints type");
    }

    cplex_constraint->setName(constraint.user_defined_name().c_str());
    _constraints.insert({ constraint.user_defined_name(), { new Constraint(constraint), cplex_constraint }});
    _model->add(*cplex_constraint);
}

void L::CplexAdapter::create_objective(const L::Objective &objective) {
    if (_lbds_objective) throw Exception("An objective has already been built");
    _lbds_objective = new Objective(objective);
    IloNumExpr cplex_expr = IloNumExpr(*_env);
    lbds_expression_to_cplex(objective.expression(), cplex_expr);
    _objective = new IloObjective(*_env, cplex_expr, to_cplex(objective.type()));
    _model->add(*_objective);
}

void L::CplexAdapter::export_to_file(const std::string &filename) {
    if (!_cplex) throw Exception("Uninitialized cplex object");
    try {
        _cplex->exportModel(filename.c_str());
    } catch (const IloException& err) {
        std::cout << err.getMessage() << std::endl;
        __throw_exception_again;
    }
}

void L::CplexAdapter::solve() {

    if (_objective == nullptr) throw Exception("Cannot solve model without objective");

    _cplex->setParam(IloCplex::RootAlg, IloCplex::Primal);
    _cplex->setParam(IloCplex::PreInd, Application::parameters().external_solver_preprocessing());

    /* _cplex->setParam(IloCplex::Param::Read::DataCheck,1);
    _cplex->setParam(IloCplex::Param::OptimalityTarget,CPX_OPTIMALITYTARGET_OPTIMALGLOBAL);
    _cplex->setParam(IloCplex::Param::Threads, 1);
    _cplex->setParam(IloCplex::Param::MIP::Strategy::VariableSelect, CPX_VARSEL_MAXINFEAS); */

    _cplex->solve();
    // std::cout << "Nodes: " << _cplex->getNnodes() << std::endl;

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
            if (found == _variables.end()) {
                _L_LOG_(Debug) << "Variable " << var.user_defined_name() << " was casted by value() to float" << std::endl;
                result += var.value();
                // throw Exception("Reference to undeclared variable");
            } else {
                result = *found->second.second;
            }
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
        case IloAlgorithm::InfeasibleOrUnbounded: throw Exception("CPLEX ended with status \"InfeasibleOrUnbounded\" ");
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

    if (_lbds_objective->status() == Feasible || _lbds_objective->status() == Optimal) {
        if (primal) {
            for (auto &m : _variables)
                m.second.first->value(_cplex->getValue(*m.second.second));
        }
        if (dual) {
            for (auto &m : _constraints)
                m.second.first->dual().value(_cplex->getDual(*m.second.second));
        }
    } else if (_lbds_objective->status() == Unbounded) {
        IloNumArray result(*_env);
        IloNumVarArray arr(*_env);
        _cplex->getRay(result, arr);
        for (auto &m : _variables) m.second.first->value(0);
        for (unsigned int i = 0, n = result.getSize() ; i < n ; i += 1) {
            _variables.find(arr[i].getName())->second.first->value(result[i]);
        }
    }
}

void L::CplexAdapter::rebuild_objective() {
    if (_objective) {
        _objective->end();
        delete _objective;
        _objective = nullptr;
    }
    Objective* obj = _lbds_objective;
    _lbds_objective = nullptr;
    create_objective(*obj);
    delete obj;
}

void L::CplexAdapter::remove_constraint(const L::Constraint &ctr) {
    auto found = _constraints.find(ctr.user_defined_name());
    if (found == _constraints.end()) throw Exception("Cannot remove undeclared constraints");
    found->second.second->end();
    delete found->second.second;
    delete found->second.first;
    _constraints.erase(found);
}

void L::CplexAdapter::rebuild_constraint(const L::Constraint &ctr) {
    // todo, use setLinCoef !
    remove_constraint(ctr);
    create_constraint(ctr);
}

