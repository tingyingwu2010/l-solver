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

    Expression expr;

    expr += 2;
    expr /= 1;
    expr *= x(1);

    Expression expr2;
    expr2 *= 3;
    expr2 += 5;

    expr += expr2;

    (expr * sin(2 * x(1))).to_dot();

    return 0;
}