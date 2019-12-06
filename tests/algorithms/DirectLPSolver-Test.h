//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_DIRECTLPSOLVER_H
#define LSOLVERPROJECT_DIRECTLPSOLVER_H

#include <LSolver/modeling/models/Model.h>
#include <LSolver/modeling/vectors/Vector.h>
#include <LSolver/modeling/vectors/VariableVector.h>
#include <LSolver/algorithms/direct-solver/DirectLPSolver.h>
#include <LSolver/adapters/CplexAdapter.h>
#include "gtest/gtest.h"

TEST(direct_lp_solver, solution_shoud_be_exact) {

    Environment env;
    Model model;
    VariableVector x = VariableVector(env, "x");
    ConstraintVector ctr = ConstraintVector(env, "ctr");
    Objective obj = Objective(env, "objective");

    obj.expression() -= x(1) + 2 * x(2) - x(3);
    ctr(1).expression() = 2 * x(1) + 1 * x(2) + x(3) - 14;
    ctr(1).type(LessOrEqualTo);
    ctr(2).expression() = 4 * x(1) + 2 * x(2) + 3 * x(3) - 28;
    ctr(2).type(LessOrEqualTo);
    ctr(3).expression() = 2 * x(1) + 5 * x(2) + 5 * x(3) - 30;
    ctr(3).type(LessOrEqualTo);

    model.add(x(1));
    model.add(x(2));
    model.add(x(3));
    model.add(ctr(1));
    model.add(ctr(2));
    model.add(ctr(3));
    model.add(obj);

    DirectLPSolver<CplexAdapter> solver(model);
    solver.solve();

    EXPECT_EQ(obj.status(), Optimal);
    EXPECT_EQ(obj.value(), -13);
    EXPECT_EQ(x(1).value(), 5);
    EXPECT_EQ(x(2).value(), 4);
    EXPECT_EQ(x(3).value(), 0);
}

TEST(direct_lp_solver, solution_should_be_unbounded) {
    Environment env;
    Model model;
    VariableVector x = VariableVector(env, "x");
    ConstraintVector ctr = ConstraintVector(env, "ctr");
    Objective obj = Objective(env, "objective");

    obj.expression() = -1 * x(1) + x(2) + x(3);
    ctr(1).expression() = x(2) + x(3) - 3;
    ctr(1).type(GreaterOrEqualTo);

    model.add(ctr(1));
    model.add(x(1));
    model.add(x(2));
    model.add(x(3));
    model.add(obj);

    DirectLPSolver<CplexAdapter> solver(model);
    solver.solve();

    EXPECT_EQ(obj.status(), Unbounded);
    EXPECT_EQ(obj.value(), std::numeric_limits<float>::lowest());
}

TEST(direct_lp_solver, solution_shoud_be_exact_relaxation_of_milp) {
    // knapsack example
    Environment env;
    Model model;
    VariableVector x = VariableVector(env, "x");
    Constraint ctr = Constraint(env, "ctr");
    Objective obj = Objective(env, "objective");

    // (profit, weight)
    const std::vector<std::pair<float, float>> items = { {2, 3}, {4, 5}, {3, 1}, {4, 2} };
    const float capacity = 6;

    unsigned int n = 0;
    for (const auto& item : items) {
        x(n).type(Binary);
        obj.expression() -= item.first * x(n);
        ctr.expression() += item.second * x(n);

        model.add(x(n));
        n += 1;
    }
    ctr.expression() -= capacity;
    ctr.type(LessOrEqualTo);

    model.add(ctr);
    model.add(obj);

    DirectLPSolver<CplexAdapter> solver(model);
    solver.solve();

    EXPECT_FLOAT_EQ(x(1).value(), 0.6);
    EXPECT_FLOAT_EQ(obj.value(), -9.4);
}

TEST(direct_lp_solver, empty_model) {
    Model model;
    EXPECT_THROW(DirectLPSolver<CplexAdapter> solver(model), Exception);
}

TEST(direct_lp_solver, infeasible) {
    Environment env;
    Model model;
    Objective obj(env, "objective");
    Constraint ctr(env, "ctr");
    Variable x(env, "x");

    ctr.expression() = x + 1;
    ctr.type(LessOrEqualTo);

    model.add(obj);
    model.add(ctr);
    model.add(x);

    DirectLPSolver<CplexAdapter> solver(model);
    solver.solve();

    EXPECT_EQ(obj.status(), Infeasible);
}

#endif //LSOLVERPROJECT_DIRECTLPSOLVER_H
