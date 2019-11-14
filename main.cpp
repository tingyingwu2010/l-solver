#include <iostream>

#include "environment/Environment.h"
#include "modeling/Variable.h"
#include "modeling/Expression.h"
#include "modeling/Model.h"
#include "adapters/CplexAdapter.h"
#include "algorithms/DirectSolver/DirectMILPSolver.h"

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
    obj.expression() = 3 * x - y;
    y.type(AbstractVariable::Binary);

    Model model;
    model.add(obj);
    model.add(ctr);
    model.add(x);
    model.add(y);

    DirectMILPSolver<CplexAdapter> solver(model);
    solver.solve();
    cout << "Execution time: " << solver.last_execution_time() << endl;
    if (obj.status() == Optimal) {
        cout << "x = " << x.value() << endl;
        cout << "y = " << y.value() << endl;
    }

    return 0;
}
