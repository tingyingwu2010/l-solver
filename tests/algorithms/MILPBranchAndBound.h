//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_MILPBRANCHANDBOUND_H
#define LSOLVERPROJECT_MILPBRANCHANDBOUND_H

#include <LSolver/algorithms/MILPBranchAndBound/MILPBranchAndBound.h>
#include <LSolver/algorithms/branch-and-bound/BranchingRules/MostInfeasibleBranchingRule.h>

TEST(branch_and_bound_milp_solver, optimal) {
    // with a small knapsack example
    Environment env;
    Model model;
    VariableVector x(env, "x");
    Constraint ctr(env, "knapsack_ctr");
    Objective obj(env, "maximize_profit");

    // (profit, weight)
    const std::vector<std::pair<float, float>> items = { {60, 10}, {100, 20}, {120, 30} };
    const unsigned int n_items = items.size();
    const float capacity = 50;

    for (unsigned int i = 0 ; i < n_items ; i += 1) {
        x(i).type(Binary);
        obj.expression() -= items[i].first * x(i);
        ctr.expression() += items[i].second * x(i);
    }
    ctr.expression() -= capacity;
    ctr.type(LessOrEqualTo);

    model.add(obj);
    model.add(ctr);
    model.add(x);

    MostInfeasibleBranchingRule rule;
    MILPBranchAndBound<CplexAdapter> solver(model);
    solver.branching_rule(rule);
    solver.solve();

    EXPECT_EQ(obj.status(), Optimal);
    EXPECT_EQ(obj.value(), -220);
    EXPECT_FLOAT_EQ(x(0).value(), 0);
    EXPECT_FLOAT_EQ(x(1).value(), 1);
    EXPECT_FLOAT_EQ(x(2).value(), 1);

}

TEST(branch_and_bound_milp_solver, infeasible) {
    Environment env;
    Model model;
    Variable x(env, "x");
    Constraint ctr(env, "infeasible");
    Objective obj(env, "zero");

    ctr.expression() = x - 0.5;
    ctr.type(EqualTo);
    x.type(Binary);

    model.add(x);
    model.add(obj);
    model.add(ctr);

    MostInfeasibleBranchingRule rule;
    MILPBranchAndBound<CplexAdapter> solver(model);
    solver.branching_rule(rule);
    solver.solve();

    EXPECT_EQ(obj.status(), Infeasible);
}

TEST(branch_and_bound_milp_solver, unbounded) {
    Environment env;
    Model model;
    Variable x(env, "x");
    Constraint ctr(env, "ctr");
    Objective obj(env, "zero");

    obj.expression() -= x;
    ctr.expression() = x - 2;
    x.type(Integer);

    model.add(x);
    model.add(obj);
    model.add(ctr);

    MostInfeasibleBranchingRule rule;
    MILPBranchAndBound<CplexAdapter> solver(model);
    solver.branching_rule(rule);
    solver.solve();

    EXPECT_EQ(obj.status(), Unbounded);
    EXPECT_EQ(obj.value(), std::numeric_limits<float>::lowest());
}

#endif //LSOLVERPROJECT_MILPBRANCHANDBOUND_H
