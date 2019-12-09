#include <iostream>
#include "RobustWNTJInstance.h"
#include "RobustWNTJBapSolve.h"

using namespace std;

int main() {
    const std::string instance_filename = "data/instance_3j_1t.dat";
    RobustWNTJInstance instance = RobustWNTJInstance(instance_filename, true);
    RobustWNTJInstance reversed = RobustWNTJInstance::reverse(instance);

    solve_by_bap(reversed);
}
