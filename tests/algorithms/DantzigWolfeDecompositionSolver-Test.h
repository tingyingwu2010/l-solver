//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_DANTZIGWOLFEDECOMPOSITION_TEST_H
#define LSOLVERPROJECT_DANTZIGWOLFEDECOMPOSITION_TEST_H

#include <LSolver/modeling/models/DualAngularModel.h>
#include <LSolver/algorithms/dantzig-wolfe/DantzigWolfeDecompositionSolver.h>

TEST(dantzig_wolfe_cg, optimal) {
    Environment env;
    VariableVector x = VariableVector(env, "x");
    VariableVector y = VariableVector(env, "y");
    ConstraintVector linking_constraints = ConstraintVector(env, "linking_constraints");
    ConstraintVector subproblem_y = ConstraintVector(env, "subproblem_y");
    ConstraintVector subproblem_x = ConstraintVector(env, "subproblem_x");
    Objective objective = Objective(env, "objective");

    // the following model has optimal solution 5.8, tested with CPLEX and direct formulation
    objective.expression() = 3 * x(0) + 6 * x(1) + 7 * x(2) + 5 * x(3) + 3 * x(4) + 5 * y(0) + 6 * y(1) + 2 * y(2) + 9 * y(3) + 1 * y(4);
    linking_constraints(0).expression() = 2 * x(0) + 7 * x(1) + 0 * x(2) + 9 * x(3) + 3 * x(4) + 6 * y(0) + 0 * y(1) + 6 * y(2) + 2 * y(3) + 6 * y(4) + -1;
    linking_constraints(1).expression() = 8 * x(0) + 7 * x(1) + 9 * x(2) + 2 * x(3) + 0 * x(4) + 2 * y(0) + 3 * y(1) + 7 * y(2) + 5 * y(3) + 9 * y(4) + -2;
    linking_constraints(2).expression() = 2 * x(0) + 8 * x(1) + 9 * x(2) + 7 * x(3) + 3 * x(4) + 6 * y(0) + 1 * y(1) + 2 * y(2) + 9 * y(3) + 3 * y(4) + -1;
    linking_constraints(3).expression() = 9 * x(0) + 4 * x(1) + 7 * x(2) + 8 * x(3) + 4 * x(4) + 5 * y(0) + 0 * y(1) + 3 * y(2) + 6 * y(3) + 1 * y(4) + 0;
    linking_constraints(4).expression() = 6 * x(0) + 3 * x(1) + 2 * x(2) + 0 * x(3) + 6 * x(4) + 1 * y(0) + 5 * y(1) + 5 * y(2) + 4 * y(3) + 7 * y(4) + -6;
    linking_constraints(5).expression() = 5 * x(0) + 6 * x(1) + 9 * x(2) + 3 * x(3) + 7 * x(4) + 4 * y(0) + 5 * y(1) + 2 * y(2) + 5 * y(3) + 4 * y(4) + -7;
    linking_constraints(6).expression() = 4 * x(0) + 4 * x(1) + 3 * x(2) + 0 * x(3) + 7 * x(4) + 8 * y(0) + 6 * y(1) + 8 * y(2) + 8 * y(3) + 4 * y(4) + -3;
    linking_constraints(7).expression() = 1 * x(0) + 4 * x(1) + 9 * x(2) + 2 * x(3) + 0 * x(4) + 6 * y(0) + 8 * y(1) + 9 * y(2) + 2 * y(3) + 6 * y(4) + -6;
    linking_constraints(8).expression() = 4 * x(0) + 9 * x(1) + 5 * x(2) + 0 * x(3) + 4 * x(4) + 8 * y(0) + 7 * y(1) + 1 * y(2) + 7 * y(3) + 2 * y(4) + -7;
    linking_constraints(9).expression() = 2 * x(0) + 2 * x(1) + 6 * x(2) + 1 * x(3) + 0 * x(4) + 6 * y(0) + 1 * y(1) + 5 * y(2) + 9 * y(3) + 4 * y(4) + -9;
    linking_constraints(10).expression() = 0 * x(0) + 9 * x(1) + 1 * x(2) + 7 * x(3) + 7 * x(4) + 1 * y(0) + 1 * y(1) + 5 * y(2) + 9 * y(3) + 7 * y(4) + -7;
    linking_constraints(11).expression() = 6 * x(0) + 7 * x(1) + 3 * x(2) + 6 * x(3) + 5 * x(4) + 6 * y(0) + 3 * y(1) + 9 * y(2) + 4 * y(3) + 8 * y(4) + -1;
    linking_constraints(12).expression() = 2 * x(0) + 9 * x(1) + 3 * x(2) + 9 * x(3) + 0 * x(4) + 8 * y(0) + 8 * y(1) + 5 * y(2) + 0 * y(3) + 9 * y(4) + -6;
    linking_constraints(13).expression() = 3 * x(0) + 8 * x(1) + 5 * x(2) + 6 * x(3) + 1 * x(4) + 1 * y(0) + 5 * y(1) + 9 * y(2) + 8 * y(3) + 4 * y(4) + -8;
    linking_constraints(14).expression() = 1 * x(0) + 0 * x(1) + 3 * x(2) + 0 * x(3) + 4 * x(4) + 4 * y(0) + 4 * y(1) + 4 * y(2) + 7 * y(3) + 6 * y(4) + -3;
    subproblem_y(0).expression() = 1 * y(0) + 7 * y(1) + 5 * y(2) + 9 * y(3) + 6 * y(4) + -2;
    subproblem_y(1).expression() = 1 * y(0) + 7 * y(1) + 8 * y(2) + 5 * y(3) + 7 * y(4) + -4;
    subproblem_y(2).expression() = 1 * y(0) + 8 * y(1) + 5 * y(2) + 9 * y(3) + 7 * y(4) + -5;
    subproblem_y(3).expression() = 3 * y(0) + 8 * y(1) + 8 * y(2) + 3 * y(3) + 1 * y(4) + -8;
    subproblem_y(4).expression() = 9 * y(0) + 6 * y(1) + 4 * y(2) + 3 * y(3) + 3 * y(4) + -3;
    subproblem_x(0).expression() = 8 * x(0) + 6 * x(1) + 0 * x(2) + 4 * x(3) + 8 * x(4) + -8;
    subproblem_x(1).expression() = 8 * x(0) + 9 * x(1) + 7 * x(2) + 7 * x(3) + 6 * x(4) + -4;
    subproblem_x(2).expression() = 5 * x(0) + 4 * x(1) + 0 * x(2) + 5 * x(3) + 9 * x(4) + -4;

    // build model
    Model model;
    model.add(objective);
    model.add(linking_constraints);
    model.add(subproblem_y);
    model.add(subproblem_x);
    model.add(x);
    model.add(y);

    // build decomposition
    Decomposition decomposition(model);
    decomposition.add_block_indicator("dw_in_y", [](const Variable& var){ return var.user_defined_name()[0] == 'y'; });
    decomposition.add_block_indicator("dw_in_x", [](const Variable& var){ return var.user_defined_name()[0] == 'x'; });

    // solve
    DantzigWolfeDecompositionSolver<CplexAdapter, DirectLPSolver<CplexAdapter>> solver(decomposition);
    solver.solve();

    EXPECT_EQ(model.objective().status(), Optimal);
    EXPECT_FLOAT_EQ(model.objective().value(), 5.8);
    EXPECT_FLOAT_EQ(x(0).value(), 0.2);
    EXPECT_FLOAT_EQ(x(1).value(), 0);
    EXPECT_FLOAT_EQ(x(2).value(), 0);
    EXPECT_FLOAT_EQ(x(3).value(), 0);
    EXPECT_FLOAT_EQ(x(4).value(), 0.8);
    EXPECT_FLOAT_EQ(y(0).value(), 0);
    EXPECT_FLOAT_EQ(y(1).value(), 0);
    EXPECT_FLOAT_EQ(y(2).value(), 0.8666667);
    EXPECT_FLOAT_EQ(y(3).value(), 0);
    EXPECT_FLOAT_EQ(y(4).value(), 1.066667);

}

TEST(dantzig_wolfe_cg, infeasible) {
    Environment env;
    VariableVector x = VariableVector(env, "x");
    VariableVector y = VariableVector(env, "y");
    ConstraintVector linking_constraints = ConstraintVector(env, "linking_constraints");
    ConstraintVector subproblem_y = ConstraintVector(env, "subproblem_y");
    ConstraintVector subproblem_x = ConstraintVector(env, "subproblem_x");
    Objective objective = Objective(env, "objective");

    // the following model has optimal solution 5.8, tested with CPLEX and direct formulation
    objective.expression() = x(0) + 5 * y(0) + 6 * y(1) + 2 * y(2) + 9 * y(3) + 1 * y(4);
    linking_constraints(0).expression() = 2 * x(0) + 7 * x(1) + 0 * x(2) + 9 * x(3) + 3 * x(4) + 6 * y(0) + 0 * y(1) + 6 * y(2) + 2 * y(3) + 6 * y(4) + -1;
    subproblem_y(0).expression() = 1 * y(0) + 7 * y(1) + 5 * y(2) + 9 * y(3) + 6 * y(4) + -2;
    subproblem_x(0).expression() = x(0) + 2;
    subproblem_x(0).type(LessOrEqualTo);

    // build model
    Model model;
    model.add(objective);
    model.add(linking_constraints);
    model.add(subproblem_y);
    model.add(subproblem_x);
    model.add(x);
    model.add(y);

    // build decomposition
    Decomposition decomposition(model);
    decomposition.add_block_indicator("dw_in_y", [](const Variable& var){ return var.user_defined_name()[0] == 'y'; });
    decomposition.add_block_indicator("dw_in_x", [](const Variable& var){ return var.user_defined_name()[0] == 'x'; });

    // solve
    DantzigWolfeDecompositionSolver<CplexAdapter, DirectLPSolver<CplexAdapter>> solver(decomposition);
    solver.solve();

    EXPECT_EQ(model.objective().status(), Infeasible);
}

TEST(dantzig_wolfe_cg, unbounded) {
    Environment env;
    VariableVector x = VariableVector(env, "x");
    VariableVector y = VariableVector(env, "y");
    ConstraintVector linking_constraints = ConstraintVector(env, "linking_constraints");
    ConstraintVector subproblem_y = ConstraintVector(env, "subproblem_y");
    ConstraintVector subproblem_x = ConstraintVector(env, "subproblem_x");
    Objective objective = Objective(env, "objective");

    // the following model has optimal solution 5.8, tested with CPLEX and direct formulation
    objective.expression() = -1 * x(0) + 2 * x(1) + 5 * y(0) + 6 * y(1) + 2 * y(2) + 9 * y(3) + 1 * y(4);
    linking_constraints(0).expression() = 2 * x(0) + 7 * x(1) + 0 * x(2) + 9 * x(3) + 3 * x(4) + 6 * y(0) + 0 * y(1) + 6 * y(2) + 2 * y(3) + 6 * y(4) + -1;
    subproblem_y(0).expression() = 1 * y(0) + 7 * y(1) + 5 * y(2) + 9 * y(3) + 6 * y(4) + -2;
    subproblem_x(0).expression() = x(1) + 2;

    // build model
    Model model;
    model.add(objective);
    model.add(linking_constraints);
    model.add(subproblem_y);
    model.add(subproblem_x);
    model.add(x);
    model.add(y);

    // build decomposition
    Decomposition decomposition(model);
    decomposition.add_block_indicator("dw_in_y", [](const Variable& var){ return var.user_defined_name()[0] == 'y'; });
    decomposition.add_block_indicator("dw_in_x", [](const Variable& var){ return var.user_defined_name()[0] == 'x'; });

    // solve
    DantzigWolfeDecompositionSolver<CplexAdapter, DirectLPSolver<CplexAdapter>> solver(decomposition);
    solver.solve();

    EXPECT_EQ(model.objective().status(), Unbounded);
    EXPECT_FLOAT_EQ(model.objective().value(), std::numeric_limits<float>::lowest());
}

TEST(dantzig_wolfe_cg, random) {
    for (unsigned int test = 0 ; test < 10 ; test += 1) {
        Environment env;
        VariableVector x = VariableVector(env, "x");
        VariableVector y = VariableVector(env, "y");
        ConstraintVector linking_constraints = ConstraintVector(env, "linking_constraints");
        ConstraintVector subproblem_y = ConstraintVector(env, "subproblem_y");
        ConstraintVector subproblem_x = ConstraintVector(env, "subproblem_x");
        Objective objective = Objective(env, "objective");

        const unsigned int n_var_x = 5 + (rand() % 30);
        const unsigned int n_var_y = 5 + (rand() % 30);
        const unsigned int n_linking_ctr = 5 + (rand() % 30);
        const unsigned int n_subprob_x_ctr = 5 + (rand() % 30);
        const unsigned int n_subprob_y_ctr = 5 + (rand() % 30);

        for (unsigned int j = 0 ; j < n_var_x ; j += 1) objective.expression() += (rand() % 10) * x(j);
        for (unsigned int j = 0 ; j < n_var_y ; j += 1) objective.expression() += (rand() % 10) * y(j);
        for (unsigned int i = 0 ; i < n_linking_ctr ; i += 1) {
            for (unsigned int j = 0 ; j < n_var_x ; j += 1) linking_constraints(i).expression() += (rand() % 10) * x(j);
            for (unsigned int j = 0 ; j < n_var_y ; j += 1) linking_constraints(i).expression() += (rand() % 10) * y(j);
            linking_constraints(i).expression() += -(rand() % 10);
        }
        for (unsigned int i = 0 ; i < n_subprob_y_ctr ; i += 1) {
            for (unsigned int j = 0 ; j < n_var_y ; j += 1) subproblem_y(i).expression() += (rand() % 10) * y(j);
            subproblem_y(i).expression() += -(rand() % 10);
        }
        for (unsigned int i = 0 ; i < n_subprob_x_ctr ; i += 1) {
            for (unsigned int j = 0 ; j < n_var_x ; j += 1) subproblem_x(i).expression() += (rand() % 10) * x(j);
            subproblem_x(i).expression() += -(rand() % 10);
        }

        Model model;
        model.add(objective);
        model.add(linking_constraints);
        model.add(subproblem_y);
        model.add(subproblem_x);
        model.add(x);
        model.add(y);

        Decomposition decomposition(model);
        decomposition.add_block_indicator("dw_in_y", [](const Variable& var){ return var.user_defined_name()[0] == 'y'; });
        decomposition.add_block_indicator("dw_in_x", [](const Variable& var){ return var.user_defined_name()[0] == 'x'; });

        DirectLPSolver<CplexAdapter> direct_solver(model);
        direct_solver.solve();
        const ObjectiveStatus cplex_status = model.objective().status();
        const float cplex_objective = model.objective().value();

        DantzigWolfeDecompositionSolver<CplexAdapter, DirectLPSolver<CplexAdapter>> dantzig_wolfe_solver(decomposition);
        dantzig_wolfe_solver.solve();
        const ObjectiveStatus dw_status = model.objective().status();
        const float dw_objective = model.objective().value();

        EXPECT_EQ(dw_status, cplex_status);
        EXPECT_FLOAT_EQ(dw_objective, cplex_objective);
    }
}

#endif //LSOLVERPROJECT_DANTZIGWOLFEDECOMPOSITION_TEST_H
