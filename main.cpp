#include <iostream>

#include "environment/Environment.h"
#include "modeling/expressions/Expression.h"
#include "modeling/expressions/LinearExpression.h"

using namespace std;

int main() {

    Environment env;
    Variable x = env.variable("x");
    Variable y = env.variable("y");
    Variable a = env.variable("a");
    Variable b = env.variable("b");
    Variable c = env.variable("c");
    c.value(10);

    Expression expr;
    expr = 2 * c * x - y + 3;

    expr.linear_factorize([](const Variable& var) { return var.user_defined_name() != "c"; });
    expr.export_to_dot("binary_tree", true);

    cout << LinearExpression::offset(expr) << endl;
    for (auto term : LinearExpression::terms(expr)) {
        cout << term.variable.user_defined_name() << " * " << term.coefficient << endl;
    }

    return 0;
}
