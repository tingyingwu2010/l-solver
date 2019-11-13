#include <iostream>

#include "environment/Environment.h"
#include "modeling/Variable.h"
#include "modeling/Expression.h"

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

    Expression z = (x + b + y) * (a + c);

    z.export_to_dot("expression_tree", true);

    return 0;
}
