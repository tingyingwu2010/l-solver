#include <iostream>

#include "environment/Environment.h"
#include "modeling/Variable.h"
#include "modeling/Expression.h"
#include "adapters/CplexAdapter.h"

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

    CplexAdapter adapter;
    adapter.create_variable(x);
    y.type(AbstractVariable::Binary);
    adapter.create_variable(y);
    adapter.create_constraint(ctr);
    adapter.create_objective(obj);
    adapter.solve();
    adapter.save_results();

    adapter.export_to_file("model.lp");

    return 0;
}
