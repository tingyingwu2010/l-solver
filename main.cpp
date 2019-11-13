#include <iostream>

#include "environment/Environment.h"
#include "modeling/Variable.h"
#include "modeling/Expression.h"

using namespace std;
using namespace L;

int main() {

    Environment env;
    Variable x = Variable(env, "x");
    Variable y = Variable(env, "y");
    Constraint ctr = Constraint(env, "ctr");
    ctr.expression() = x + y;

    return 0;
}
