//
// Created by hlefebvr on 22/11/19.
//

#ifndef LSOLVERPROJECT_DANTZIGWOLFEMODEL_H
#define LSOLVERPROJECT_DANTZIGWOLFEMODEL_H

#include <LSolver/modeling/models/Decomposition.h>
#include <LSolver/environment/Environment.h>
#include <LSolver/modeling/objectives/DetachedObjective.h>

namespace L {
    class DantzigWolfeModel;
    class DantzigWolfeSubProblem;
}

class L::DantzigWolfeSubProblem : public Model {
    Environment _env;
    Objective _objective;
    std::vector<std::pair<Constraint, Expression>> _linking_constraints;
    Expression _linking_cost;
    Constraint* _convex_constraint = nullptr;
    typedef std::vector<std::pair<Constraint, Expression>> LinkingConstraintExpression;
public:
    virtual ~DantzigWolfeSubProblem();
    explicit DantzigWolfeSubProblem(const std::string& user_defind_name);
    void add_linking_expression(const Constraint& ctr, const Expression& expr);
    void set_linking_cost(const Expression& expr);
    [[nodiscard]] const LinkingConstraintExpression& linking_constraint_expressions() const;
    Constraint convex_constraint();
    Expression& linking_cost();
    void set_convex_constraint(const Constraint& ctr);
};

class L::DantzigWolfeModel {
    unsigned int _n_artificial_var = 0;
    Decomposition& _decomposition;
    Environment _env;
    Model _restricted_master_problem = Model("_RMP");
    std::map<std::string, DantzigWolfeSubProblem*> _sub_problems;
    std::vector<DetachedConstraint*> _detached_linking_constraints;

    /**
     * Returns true if the given splitted expression represents a linking constraint, if not however, the block_name argument is
     * changed to the block which is defined by the constraint
     * @param split splitted constraint's expression
     * @param[out] block_name
     * @return true if the given splitted expression represents a linking constraint
     */
    bool is_linking_constraint(const std::map<std::string, Expression>& split, std::string& block_name);
    void dispatch();
    void dispatch(Variable& variable);
    void dispatch(Constraint& ctr);
    void dispatch(Objective obj);
    void add_convex_constraints();
    void add_artificial_variables(Constraint& ctr);
public:
    virtual ~DantzigWolfeModel();
    explicit DantzigWolfeModel(Decomposition& decomposition);
    Model& restricted_master_problem();
    Environment& env();
    const std::map<std::string, DantzigWolfeSubProblem*>& sub_problems();
};


#endif //LSOLVERPROJECT_DANTZIGWOLFEMODEL_H
