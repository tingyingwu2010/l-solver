#include <iostream>

#include "environment/Environment.h"

using namespace std;
using namespace ED;

int main() {
    Environment env;
    env.variable("x");
    env.variable("y");
    env.constraint("ctr");

    for (Variable v : env.variables()) cout << v << endl;
    for (Constraint ctr : env.constraints()) cout << ctr << endl;

    return 0;
}
