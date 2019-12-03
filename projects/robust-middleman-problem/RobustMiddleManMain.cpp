#include <iostream>
#include <LSolver/environment/Environment.h>
#include <LSolver/modeling/models/Model.h>
#include <LSolver/modeling/vectors/VariableVector.h>
#include <LSolver/modeling/models/Decomposition.h>
#include <LSolver/adapters/CplexAdapter.h>
#include <LSolver/algorithms/direct-solver/DirectMILPSolver.h>
#include <LSolver/algorithms/dantzig-wolfe-branch-and-price/DantzigWolfeBranchAndPrice.h>
#include <LSolver/algorithms/branch-and-bound/branching-rules/MostInfeasibleBranchingRule.h>
#include "RobustMiddlemanInstance.h"

using namespace std;
using namespace L;

int main(int argc, char** argv) {

    if (argc != 4) throw Exception("Usage: ./RobustMiddlemanMain <instance_file> <gamma> <budget>");

    const std::string instance_file = argv[1];
    const float gamma = std::stof(argv[2]);
    const float budget = std::stof(argv[3]);
    RobustMiddlemanInstance instance(instance_file, gamma, budget);

    // build components
    Environment env;
    Variable u(env, "u");
    VariableVector v(env, "v");
    VariableVector x(env, "x");
    VariableVector y(env, "y", 2);
    ConstraintVector recourse(env, "recourse");
    ConstraintVector dual_cost(env, "dual_cost");
    Constraint first_stage(env, "first_stage");
    Objective obj(env, "objective");

    x.priority(1);
    y.type(Binary);
    y.priority(0);

    obj.expression() += instance.gamma() * u;
    for (const Offer& offer : instance.offers()) {
        obj.expression() += offer.buy_price() * x(offer.id()) + v(offer.id());
        first_stage.expression() += offer.buy_price() * x(offer.id());
        dual_cost(offer.id()).expression() += u + v(offer.id());
        recourse(offer.id()).expression() -= x(offer.id());
        recourse(offer.id()).type(LessOrEqualTo);
        for (const Demand& demand : instance.demands()) {
            obj.expression() -= offer.sell_highest_price() * y(offer.id(), demand.id());
            dual_cost(offer.id()).expression() -= (offer.sell_highest_price() - offer.sell_lowest_price()) * y(offer.id(), demand.id());
            recourse(offer.id()).expression() += demand.amount() * y(offer.id(), demand.id());
        }
    }
    first_stage.expression() -= instance.budget();
    first_stage.type(LessOrEqualTo);

    // build model
    Model model("middleman");
    model.add(obj);
    model.add(x);
    model.add(y);
    model.add(u);
    model.add(v);
    model.add(first_stage);
    model.add(dual_cost);
    model.add(recourse);

    // describe decomposition
    Decomposition decomposition(model);
    decomposition.add_block_indicator("recourse", [](const Variable& var){ return var.user_defined_name()[0] == 'y' || var.user_defined_name()[1] == 'x'; });

    // solve
    MostInfeasibleBranchingRule branching_rule;
    DantzigWolfeBranchAndPrice<CplexAdapter, DirectMILPSolver<CplexAdapter>> solver(decomposition);
    solver.branching_rule(branching_rule);
    solver.solve();

    cout << model << endl;

    cout << "Status: " << obj.status() << endl;
    cout << "Time  : " << solver.last_execution_time() << endl;
    if (obj.status() == Optimal) {
        cout << "Objective = " << obj.value() << endl;
        for (const Variable& var : x.components()) cout << var.user_defined_name() << " = " << var.value() << endl;
        for (const Variable& var : y.components()) cout << var.user_defined_name() << " = " << var.value() << endl;
        for (const Variable& var : v.components()) cout << var.user_defined_name() << " = " << var.value() << endl;
        cout << "u = " << u.value() << endl;
    }

    return 0;
}
