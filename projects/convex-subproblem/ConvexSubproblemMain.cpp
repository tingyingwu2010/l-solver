#include <iostream>
#include <fstream>
#include <LSolver/environment/Environment.h>
#include <LSolver/modeling/models/Model.h>
#include <LSolver/modeling/vectors/VariableVector.h>
#include <LSolver/algorithms/direct-solver/DirectMILPSolver.h>
#include <LSolver/adapters/CplexAdapter.h>
#include <LSolver/modeling/models/Decomposition.h>
#include <LSolver/algorithms/dantzig-wolfe/DantzigWolfeDecompositionSolver.h>
#include <LSolver/plots/Plot.h>
#include <LSolver/plots/Surface.h>

using namespace std;
using namespace L;

int main() {

    Application::load_configuration_file("configuration.cfg");

    Environment env;
    VariableVector x(env, "x");
    VariableVector y(env, "y");
    VariableVector q(env, "q");
    Variable w(env, "w");
    ConstraintVector ctr(env, "ctr");
    Objective obj(env, "objective");

    y.type(Binary);

    obj.expression() = y(1) + y(2) + q(1) + q(1);
    ctr(1).expression() = y(1) + y(2) - q(1);
    ctr(2).expression() = y(2) - q(2);

    ctr(3).type(EqualTo);
    ctr(3).expression() = q(1) - x(1);
    ctr(4).type(EqualTo);
    ctr(4).expression() = q(2) - x(2);

    Model model;
    model.add(y);
    model.add(q);
    model.add(ctr);
    model.add(obj);

    MeshPoints mesh;
    for (float x1 = 0 ; x1 <= 1 ; x1 += 0.05) {
        x(1).value(x1);
        for (float x2 = 0; x2 <= 1; x2 += 0.05) {
            x(2).value(x2);

            // DirectMILPSolver<CplexAdapter> solver(model);
            Decomposition decomposition(model);
            decomposition.add_block_indicator("all", [](const Variable &var) {
                return var.user_defined_name()[0] == 'y' || var.user_defined_name()[0] == 'q';
            });
            DantzigWolfeDecompositionSolver<CplexAdapter, DirectMILPSolver<CplexAdapter>> solver(decomposition);
            solver.solve();

            mesh.add_point(x1, x2, obj.value());
        }
    }

    Plot plot("Dantzig-Wolfe subproblem");
    std::vector<std::pair<int, int>> views = { {0,90}, { 45, 45 }, { 0, 0 }, { 90, 0 }, { 10, 45 }, { 100, 45 } };
    unsigned int i = 1, j = 1;
    for (const auto& angle : views) {
        if (j > 2) { j = 1; i += 1; }
        Surface s(mesh);
        s.rotation(angle.first, angle.second);
        s.xlabel("$x_1$");
        s.ylabel("$x_2$");
        s.title("Subproblem with angle " + std::to_string(angle.first) + "/" + std::to_string(angle.second));
        plot.add_subplot(i, j, s);
        j += 1;
    }
    plot.render("test.png");

    return 0;
}