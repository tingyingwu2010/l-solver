#include <iostream>

#include "src/environment/Environment.h"
#include "src/modeling/Variable.h"
#include "src/modeling/Expression.h"
#include "src/modeling/Model.h"
#include "src/adapters/CplexAdapter.h"
#include "src/algorithms/DirectSolver/DirectLPSolver.h"
#include "src/modeling/Vector.h"

using namespace std;
using namespace L;

int main() {

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

    DirectLPSolver<CplexAdapter> solver(model);
    solver.solve();
    cout << "Execution time: " << solver.last_execution_time() << endl;
    if (obj.status() == Optimal) {
        cout << "x = " << x.value() << endl;
        cout << "y = " << y.value() << endl;
    }

    return 0;
}
