#include <iostream>

#include "environment/Environment.h"
#include "modeling/Variable.h"
#include "modeling/VariableVector.h"
#include "modeling/Expression.h"

using namespace std;
using namespace ED;

int main() {
    Environment env;
    VariableVector x(env, "x");
    x(1);
    x(2);

    for (Variable& t : x) cout << t.core() << endl;

    // Expression expr = (x(1) - 1) * (x(2) + 2);
    Expression expr;
    expr = 1.0;
    expr.expand();
    expr.reduce();
    expr.to_dot();

    cout << expr.to_string() << endl;

    return 0;
}