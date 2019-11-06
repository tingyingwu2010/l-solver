#include <iostream>

#include "environment/Environment.h"

using namespace std;
using namespace ED;

int main() {

    Environment env;
    Expression expr;
    Variable x = env.variable("x");
    Variable y = env.variable("y");

    expr = pow(pow(x + y, 2), 1);

    expr.expand();

    expr.to_dot();
    system("dot -Tpng expression_tree.dot > image.png");

    cout << expr.to_string() << endl;

    return 0;
}
