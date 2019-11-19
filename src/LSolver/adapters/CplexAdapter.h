//
// Created by hlefebvr on 12/11/19.
//

#ifndef ED_SOLVER_CPLEXADAPTER_H
#define ED_SOLVER_CPLEXADAPTER_H

#include <map>
#include <ilcplex/ilocplex.h>
#include "SolverAdapter.h"

namespace L {
    class CplexAdapter;
    class LogManager;
}

/**
 * \brief Adapter for the IBM Cplex solver, for MILP models
 */
class L::CplexAdapter : public SolverAdapter {
    template<class LBDS, class Cplex> struct Match;
    typedef Match<Variable, IloNumVar> VariableMatch;
    typedef Match<Constraint, IloRange> ConstraintMatch;
    IloEnv* _env = new IloEnv();
    IloModel* _model = new IloModel(*_env);
    IloCplex* _cplex = new IloCplex(*_model);
    std::map<std::string, std::pair<Variable*, IloNumVar*>> _variables;
    std::map<std::string, std::pair<Constraint*, IloRange*>> _constraints;
    IloObjective* _objective = nullptr;
    Objective* _lbds_objective = nullptr;
    LogManager* _logger = nullptr;

    void lbds_expression_to_cplex(const Expression& lbds_expr, IloNumExpr& cplex_expr);
public:
    CplexAdapter();
    ~CplexAdapter();
    void rebuild_objective();
    void create_variable(const Variable& variable) override;
    void create_constraint(const Constraint& constraint) override;
    void remove_constraint(const Constraint& constraint);
    void create_objective(const Objective& objective) override;
    void rebuild_constraint(const Constraint& ctr);
    void export_to_file(const std::string& filename) override;
    void solve() override;
    void save_more_results(bool primal = true, bool dual = true, bool reduced_costs = true, bool slacks = true);
    void save_results(bool primal = true, bool dual = false) override;

    static IloNumVar::Type to_cplex(VariableType type);
    static IloObjective::Sense to_cplex(ObjectiveType type);
    static ObjectiveStatus to_lbds(IloAlgorithm::Status status);
};


#endif //ED_SOLVER_CPLEXADAPTER_H
