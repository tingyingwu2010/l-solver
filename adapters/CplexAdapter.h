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
    std::map<std::string, std::pair<Variable, IloNumVar*>> _variables;
    std::map<std::string, std::pair<Constraint, IloRange*>> _constraints;
    IloObjective* _objective = nullptr;

    void lbds_expression_to_cplex(const Expression& lbds_expr, IloNumExpr& cplex_expr);
public:
    ~CplexAdapter();
    void create_variable(const Variable& variable) override;
    void create_constraint(const Constraint& constraint) override;
    void export_to_file(const std::string& filename) override;
    void solve() override;

    static IloNumVar::Type to_cplex(AbstractVariable::Type type);
};


#endif //ED_SOLVER_CPLEXADAPTER_H
