#include <iostream>

#include "src/environment/Environment.h"
#include "src/modeling/Variable.h"
#include "src/modeling/Expression.h"
#include "src/modeling/Model.h"
#include "src/adapters/CplexAdapter.h"
#include "src/algorithms/DirectSolver/DirectLPSolver.h"
#include "src/algorithms/DirectSolver/DirectMILPSolver.h"
#include "src/algorithms/MILPBranchAndBound/MILPBranchAndBound.h"
#include "src/algorithms/BranchAndBound/BranchingRules/MostInfeasibleBranchingRule.h"
#include "src/modeling/Vector.h"

using namespace std;
using namespace L;

int main() {

    Application::load_configuration_file("../main.cfg");

    Environment env;
    Model model;
    VariableVector x = VariableVector(env, "x");
    Constraint knapsack_ctr = Constraint(env, "knapsack_ctr");
    Objective obj = Objective(env, "objective");

    for (unsigned int i = 0 ; i < 10 ; i += 1) {
        x(i).type(AbstractVariable::Binary);
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
