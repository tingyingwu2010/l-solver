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
    ConstraintVector subproblem = ConstraintVector(env, "subproblem");
    Objective objective = Objective(env, "objective");

    const unsigned int n_masterp_var = 10;
    const unsigned int n_subprob_var = 10;
    const unsigned int n_linking_ctr = 5;
    const unsigned int n_subprob_ctr = 5;

    model.add(objective);
    for (unsigned int j = 0 ; j < n_masterp_var ; j += 1) {
        model.add(x(j));
        objective.expression() += (rand() % 10) * x(j);
    }
    for (unsigned int j = 0 ; j < n_subprob_var ; j += 1) {
        model.add(y(j));
        objective.expression() += (rand() % 10) * y(j);
    }
    for (unsigned int i = 0 ; i < n_linking_ctr ; i += 1) {
        model.add(linking_constraints(i));
        for (unsigned int j = 0 ; j < n_masterp_var ; j += 1) linking_constraints(i).expression() += (rand() % 10) * x(j);
        for (unsigned int j = 0 ; j < n_subprob_var ; j += 1) linking_constraints(i).expression() += (rand() % 10) * y(j);
        linking_constraints(i).expression() += -(rand() % 10);
        linking_constraints(i).type(GreaterOrEqualTo);
    }
    for (unsigned int i = 0 ; i < n_subprob_ctr ; i += 1) {
        model.add(subproblem(i));
        for (unsigned int j = 0 ; j < n_subprob_var ; j += 1) subproblem(i).expression() += (rand() % 10) * y(j);
        subproblem(i).expression() += -(rand() % 10);
        subproblem(i).type(GreaterOrEqualTo);
    }

    DirectLPSolver<CplexAdapter> direct_solver(model);
    direct_solver.solve();
    cout << "Status: " << objective.status() << endl;

    DantzigWolfeDecomposition<CplexAdapter> dw_solver(model);
    dw_solver.add_decomposition("dw_in_y", [](const Variable& var){ return var.user_defined_name()[0] == 'y'; });
    dw_solver.decompose();

    return 0;
}
