#include <iostream>

#include <LSolver/environment/Environment.h>
#include <LSolver/modeling/Variable.h>
#include <LSolver/modeling/Expression.h>
#include <LSolver/modeling/Model.h>
#include <LSolver/adapters/CplexAdapter.h>
#include <LSolver/algorithms/DirectSolver/DirectLPSolver.h>
#include <LSolver/modeling/Vector.h>
#include <LSolver/algorithms/DantzigWolfeDecomposition/DantzigWolfeDecomposition.h>

using namespace std;
using namespace L;

int main() {

    Application::load_configuration_file("../../../examples/DantzigWolfeDecompositionExample/dw.cfg");

    Environment env;
    Model model;
    VariableVector x = VariableVector(env, "x");
    VariableVector y = VariableVector(env, "y");
    ConstraintVector linking_constraints = ConstraintVector(env, "linking_constraints");
    ConstraintVector subproblem_y = ConstraintVector(env, "subproblem_y");
    ConstraintVector subproblem_x = ConstraintVector(env, "subproblem_x");
    Objective objective = Objective(env, "objective");

    const unsigned int n_var_x = 5;
    const unsigned int n_var_y = 5;
    const unsigned int n_linking_ctr = 15;
    const unsigned int n_subprob_ctr = 5;

    model.add(objective);
    for (unsigned int j = 0 ; j < n_var_x ; j += 1) {
        model.add(x(j));
        objective.expression() += (rand() % 10) * x(j);
    }
    for (unsigned int j = 0 ; j < n_var_y ; j += 1) {
        model.add(y(j));
        objective.expression() += (rand() % 10) * y(j);
    }
    for (unsigned int i = 0 ; i < n_linking_ctr ; i += 1) {
        model.add(linking_constraints(i));
        for (unsigned int j = 0 ; j < n_var_x ; j += 1) linking_constraints(i).expression() += (rand() % 10) * x(j);
        for (unsigned int j = 0 ; j < n_var_y ; j += 1) linking_constraints(i).expression() += (rand() % 10) * y(j);
        linking_constraints(i).expression() += -(rand() % 10);
        linking_constraints(i).type(GreaterOrEqualTo);
    }
    for (unsigned int i = 0 ; i < n_subprob_ctr ; i += 1) {
        model.add(subproblem_y(i));
        for (unsigned int j = 0 ; j < n_var_y ; j += 1) subproblem_y(i).expression() += (rand() % 10) * y(j);
        subproblem_y(i).expression() += -(rand() % 10);
        subproblem_y(i).type(GreaterOrEqualTo);
    }
    for (unsigned int i = 0 ; i < n_subprob_ctr ; i += 1) {
        model.add(subproblem_x(i));
        for (unsigned int j = 0 ; j < n_var_x ; j += 1) subproblem_x(i).expression() += (rand() % 10) * x(j);
        subproblem_x(i).expression() += -(rand() % 10);
        subproblem_x(i).type(GreaterOrEqualTo);
    }

    std::cout << "CPLEX direct" << std::endl;
    DirectLPSolver<CplexAdapter> direct_solver(model);
    direct_solver.solve();
    direct_solver.export_to_file("direct.lp");
    std::cout << "Status: " << objective.status() << std::endl;
    std::cout << "Time  : " << direct_solver.last_execution_time() << std::endl;
    if (objective.status() == Optimal) {
        std::cout << "obj = " << objective.value() << std::endl;
    }

    std::cout << "\n";

    std::map<std::string, std::function<bool(const Variable&)>> indicators;
    indicators.insert({ "dw_in_y", [](const Variable& var){ return var.user_defined_name()[0] == 'y'; } });
    indicators.insert({ "dw_in_x", [](const Variable& var){ return var.user_defined_name()[0] == 'x'; } });
    DualAngularModel da_model(model, indicators);
    DantzigWolfeDecomposition<CplexAdapter> dw_2(da_model);
    dw_2.solve();
    std::cout << "Column Generation / Dantzig Wolfe" << std::endl;
    std::cout << "Status: " << model.objective().status() << std::endl;
    std::cout << "Time  : " << dw_2.last_execution_time() << std::endl;
    if (objective.status() == Optimal) {
        std::cout << "obj = " << objective.value() << std::endl;
    }

    return 0;
}
