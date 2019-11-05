#include <iostream>

#include "environment/Environment.h"
#include "modeling/VariableVector.h"
#include "modeling/Expression.h"
#include "modeling/ConstraintVector.h"

using namespace std;
using namespace ED;

int main() {
    Environment env;
    VariableVector x(env, "x");
    ConstraintVector ctr(env, "ctr");

    ctr(1).expression(x(1) * 2 + 10);
    ctr(2).expression(x(1) + x(2) + 10 * cos(10 * x(1)));

    cout << ctr;

    return 0;
}
