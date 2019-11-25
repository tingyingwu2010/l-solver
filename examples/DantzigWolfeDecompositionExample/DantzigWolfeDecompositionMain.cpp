#include <iostream>

#include <LSolver/environment/Environment.h>
#include <LSolver/modeling/variables/Variable.h>
#include <LSolver/modeling/expressions/Expression.h>
#include <LSolver/modeling/models/Model.h>
#include <LSolver/adapters/CplexAdapter.h>
#include <LSolver/algorithms/DirectSolver/DirectLPSolver.h>
#include <LSolver/modeling/vectors/Vector.h>
#include <LSolver/algorithms/DantzigWolfeSolver/DantzigWolfeModel.h>
#include <LSolver/algorithms/DantzigWolfeSolver/DantzigWolfeDecompositionAlgorithm.h>
#include <LSolver/algorithms/DantzigWolfeSolver/DantzigWolfeDecompositionSolver.h>

using namespace std;
using namespace L;

int main() {

    Application::load_configuration_file("../../../examples/DantzigWolfeDecompositionExample/dw.cfg");

    Environment env;
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

    for (unsigned int j = 0 ; j < n_var_x ; j += 1) objective.expression() += (rand() % 10) * x(j);
    for (unsigned int j = 0 ; j < n_var_y ; j += 1) objective.expression() += (rand() % 10) * y(j);
    for (unsigned int i = 0 ; i < n_linking_ctr ; i += 1) {
        for (unsigned int j = 0 ; j < n_var_x ; j += 1) linking_constraints(i).expression() += (rand() % 10) * x(j);
        for (unsigned int j = 0 ; j < n_var_y ; j += 1) linking_constraints(i).expression() += (rand() % 10) * y(j);
        linking_constraints(i).expression() += -(rand() % 10);
    }
    for (unsigned int i = 0 ; i < n_subprob_ctr ; i += 1) {
        for (unsigned int j = 0 ; j < n_var_y ; j += 1) subproblem_y(i).expression() += (rand() % 10) * y(j);
        subproblem_y(i).expression() += -(rand() % 10);
    }
    for (unsigned int i = 0 ; i < n_subprob_ctr ; i += 1) {
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

    std::cout << "CPLEX direct" << std::endl;
    DirectLPSolver<CplexAdapter> direct_solver(model);
    direct_solver.solve();
    direct_solver.export_to_file("direct.lp");
    std::cout << "Status: " << objective.status() << std::endl;
    std::cout << "Time  : " << direct_solver.last_execution_time() << std::endl;
    if (objective.status() == Optimal) {
        std::cout << "obj = " << objective.value() << std::endl;
        for (const Variable& _x : env.variables())
            std::cout << _x.user_defined_name() << " = " << _x.value() << std::endl;
    }

    std::cout << "\n";

    Decomposition decomposition(model);
    decomposition.add_block_indicator("dw_in_y", [](const Variable& var){ return var.user_defined_name()[0] == 'y'; });
    decomposition.add_block_indicator("dw_in_x", [](const Variable& var){ return var.user_defined_name()[0] == 'x'; });

    DantzigWolfeDecompositionSolver<CplexAdapter, DirectLPSolver<CplexAdapter>> solver(decomposition);
    solver.solve();

    std::cout << "Column Generation / Dantzig Wolfe" << std::endl;
    std::cout << "Status: " << model.objective().status() << std::endl;
    std::cout << "Time  : " << solver.last_execution_time() << std::endl;
    if (model.objective().status() == Optimal) {
        std::cout << "obj = " << model.objective().value() << std::endl;
        for (const Variable& _x : env.variables())
            std::cout << _x.user_defined_name() << " = " << _x.value() << std::endl;
    }

    return 0;
}
