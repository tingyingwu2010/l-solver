#include <iostream>

#include "environment/Environment.h"
#include "modeling/expressions/Expression.h"
#include "modeling/expressions/LinearExpression.h"

using namespace std;
using namespace L;

int main() {

    Environment env;
    Variable x = env.variable("x");
    Variable y = env.variable("y");
    Variable a = env.variable("a");
    Variable b = env.variable("b");
    Variable c = env.variable("c");
    c.value(10);

    Constraint ctr = env.constraint("ctr");
    ctr.expression(2 * c * x  - y + 3);
    for (auto term : LinearExpression::terms(ctr.expression())) {
        cout << term.variable.user_defined_name() << " * " << term.coefficient << endl;
    }

    return 0;
}
