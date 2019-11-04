#include <iostream>

#include "environment/Environment.h"
#include "modeling/Variable.h"
#include "modeling/VariableVector.h"

using namespace std;
using namespace ED;

int main() {
    Environment env;
    VariableVector x(env, "x");
    x(1);
    x(2);

    for (Variable& t : x) cout << t.core() << endl;

    return 0;
}