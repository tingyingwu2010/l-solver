#include <iostream>
#include "QuadraticKnapsackInstance.h"
#include <LSolver/modeling/models/Model.h>
#include <LSolver/modeling/vectors/VariableVector.h>
#include <LSolver/environment/Environment.h>
#include <LSolver/algorithms/direct-solver/DirectMILPSolver.h>
#include <LSolver/adapters/CplexAdapter.h>
#include <LSolver/application/Application.h>

using namespace std;
using namespace L;

int main() {
    Application::load_configuration_file("config.cfg");

    QuadraticKnapsackInstance instance("data/randomQMKP_20_25_1.txt");
    const unsigned int n_kp = 3;
    const float capacity = .8 * instance.sum_of_weights() / ((float) n_kp);

    Environment env;
    VariableVector x(env, "x", 2);
    Objective obj(env, "minimize_cost");
    ConstraintVector kp(env, "kp");
    ConstraintVector only_one(env, "only_one");

    x.type(Binary);
    obj.type(Maximize);

    for (unsigned int k = 1 ; k <= n_kp ; k += 1) {

        for (const auto& m : instance.profits()) {
            unsigned int i = m.first.first;
            unsigned int j = m.first.second;
            float p = m.second;
            if (i == j) obj.expression() += p * x(i, k);
            else obj.expression() += p * x(i, k) * x(j, k);
        }

        kp(k).type(LessOrEqualTo);
        for (const auto &m : instance.weights()) kp(k).expression() += m.second * x(m.first, k);
        kp(k).expression() -= capacity;
    }

    for (unsigned int i = 1 ; i <= instance.n_items() ; i += 1 ) {
        only_one(i).type(LessOrEqualTo);
        only_one(i).expression() -= 1;
        for (unsigned int k = 1; k <= n_kp; k += 1) {
            only_one(i).expression() += x(i, k);
        }
    }

    Model model;
    model.add(x);
    model.add(kp);
    model.add(obj);
    model.add(only_one);

    DirectMILPSolver<CplexAdapter> solver(model);
    solver.solve();

    std::cout << "CPLEX statuts: " << obj.status() << std::endl;
    std::cout << "Execution time: " << solver.last_execution_time() << std::endl;
    if (obj.status() == Optimal) {
        std::cout << "Objective = " << obj.value() << std::endl;
    }

    return 0;
}
