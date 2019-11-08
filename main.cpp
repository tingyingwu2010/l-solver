#include <iostream>

#include "environment/Environment.h"

using namespace std;
using namespace ED;

int main() {

    Environment env;
    Expression expr;
    Variable x = env.variable("x");
    Variable y = env.variable("y");
    Variable a = env.variable("a");
    Variable b = env.variable("b");
    Variable c = env.variable("c");

    // expr = pow(x + 2, 3);
    // expr = x + 2 *  y + 1 + 2 + x * cos(pow(x + 2, 2));
    // expr = 2 * x + y + 3 * x - y;
    // expr = x + 2 * x + 2 * y + 3 * x - y;
    // expr = y + y + y;
    expr = x * a + y * b + 2 * x + 2 * c + 3 * y * b;

    a.value(10);
    b.value(2);
    c.value(2);

    expr.expand();
    expr.reduce();
    expr.linear_group_by([](const Variable& var){
        return (var.user_defined_name() == "x" || var.user_defined_name() == "y");
    });
    expr.reduce();
    expr = expr.evaluate([](const Variable& var){ return var.user_defined_name() != "a" && var.user_defined_name() != "b" && var.user_defined_name() != "c"; });

    expr.to_dot();
    cout << expr.to_string() << endl;
    cout << "processing image....";
    system("dot -Tpng expression_tree.dot > image.png");
    cout << "done!" << endl;

    return 0;
}
