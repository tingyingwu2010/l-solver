//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_DUALANGULARMODELTEST_H
#define LSOLVERPROJECT_DUALANGULARMODELTEST_H

#include <LSolver/modeling/DualAngularModel.h>

TEST(dual_angular_model, nominal_case) {
    Environment env;
    VariableVector x = VariableVector(env, "x");
    VariableVector y = VariableVector(env, "y");
    ConstraintVector linking_constraints = ConstraintVector(env, "linking_constraints");
    ConstraintVector subproblem_y = ConstraintVector(env, "subproblem_y");
    ConstraintVector subproblem_x = ConstraintVector(env, "subproblem_x");
    Objective objective = Objective(env, "objective");

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

    Model model;
    model.add(objective);
    model.add(linking_constraints);
    model.add(subproblem_y);
    model.add(subproblem_x);
    model.add(x);
    model.add(y);

    std::map<std::string, std::function<bool(const Variable&)>> indicators;
    indicators.insert({ "dw_in_y", [](const Variable& var){ return var.user_defined_name()[0] == 'y'; } });
    indicators.insert({ "dw_in_x", [](const Variable& var){ return var.user_defined_name()[0] == 'x'; } });
    DualAngularModel da_model(model, indicators);

    std::cout << da_model << std::endl;
}

#endif //LSOLVERPROJECT_DUALANGULARMODELTEST_H
