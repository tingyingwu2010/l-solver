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

    (x(1) + x(3) * 2 * x(2) * pow(1 + x(0), x(1)) + sin(x(2) + 3)).to_dot();

    return 0;
}