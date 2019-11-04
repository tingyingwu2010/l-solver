#include <iostream>

#include "environment/Environment.h"
#include "modeling/Variable.h"
#include "modeling/VariableVector.h"

using namespace std;

int main() {
    ED::Environment env;
    ED::VariableVector x(env, "x");
    x(1);
    x(2);
    cout << x << endl;
    return 0;
}