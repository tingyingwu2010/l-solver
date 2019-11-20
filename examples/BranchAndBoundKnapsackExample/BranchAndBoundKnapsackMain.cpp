//
// Created by hlefebvr on 16/11/19.
//

#include <iostream>

#include <LSolver/application/LogManager.h>
#include <LSolver/environment/Environment.h>
#include <LSolver/modeling/models/Model.h>
#include <LSolver/modeling/vectors/Vector.h>
#include <LSolver/algorithms/MILPBranchAndBound/MILPBranchAndBound.h>
#include <LSolver/algorithms/BranchAndBound/BranchingRules/MostInfeasibleBranchingRule.h>
#include <LSolver/algorithms/DirectSolver/DirectMILPSolver.h>
#include <LSolver/adapters/CplexAdapter.h>

using namespace std;
using namespace L;

int main() {

    Environment env;
    Model model;
    VariableVector x = VariableVector(env, "x");
    Constraint knapsack_ctr = Constraint(env, "knapsack_ctr");
    Objective obj = Objective(env, "objective");

    for (unsigned int i = 0 ; i < 10 ; i += 1) {
        x(i).type(Binary);
        model.add(x(i));
        obj.expression() += -(rand() % 10) * x(i);
        knapsack_ctr.expression() += (rand() % 10) * x(i);
    }
    knapsack_ctr.expression() += -12;
    model.add(obj);
    model.add(knapsack_ctr);

    MostInfeasibleBranchingRule rule;
    MILPBranchAndBound<CplexAdapter> solver(model);
    solver.branching_rule(rule);
    solver.solve();
    std::cout << "L: status " << model.objective().status() << ", obj = " << model.objective().value() << std::endl;

    DirectMILPSolver<CplexAdapter> direct_solver(model);
    direct_solver.solve();
    std::cout << "CPLEX: status " << model.objective().status() << ", obj = " << model.objective().value() << std::endl;

    return 0;
}