#include <iostream>

#include "environment/Environment.h"

using namespace std;
using namespace ED;

int main() {

    Environment env;
    Expression expr;
    Variable x = env.variable("x");
    Variable y = env.variable("y");

    expr = pow(x + 2, 3);
    // expr = x + 2 *  y + 1 + 2 + x * cos(pow(x + 2, 2));

    expr.expand();
    expr.reduce();

    expr.to_dot();
    cout << expr.to_string() << endl;
    cout << "processing image....";
    system("dot -Tpng expression_tree.dot > image.png");
    cout << "done!" << endl;

    return 0;
}
