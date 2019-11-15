#include <iostream>

#include "src/environment/Environment.h"
#include "src/modeling/Variable.h"
#include "src/modeling/Expression.h"
#include "src/modeling/Model.h"
#include "src/adapters/CplexAdapter.h"
#include "src/algorithms/DirectSolver/DirectLPSolver.h"
#include "src/algorithms/MILPBranchAndBound/MILPBranchAndBound.h"
#include "src/algorithms/BranchAndBound/BranchingRules/MostInfeasibleBranchingRule.h"

using namespace std;
using namespace L;

int main() {

    Application::load_configuration_file("../main.cfg");

    Environment env;
    Variable x = Variable(env, "x");
    Variable y = Variable(env, "y");
    Constraint ctr = Constraint(env, "ctr");
    ctr.expression() = 3 * x + 2 * y - 10;
    ctr.type(L::Constraint::GreaterOrEqualTo);
    Objective obj = Objective(env, "objective");
    obj.expression() = 3 * x + y;
    y.type(AbstractVariable::Binary);

    Model model;
    model.add(obj);
    model.add(ctr);
    model.add(x);
    model.add(y);

    MostInfeasibleBranchingRule rule;
    MILPBranchAndBound<CplexAdapter> solver(model);
    solver.branching_rule(rule);
    solver.solve();

    std::cout << "status " << model.objective().status() << std::endl;
    for (const auto& var : model.variables())
        std::cout << var.user_defined_name() << " = " << var.value() << std::endl;

    return 0;
}
