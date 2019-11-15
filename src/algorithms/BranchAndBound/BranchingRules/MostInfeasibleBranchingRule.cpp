//
// Created by hlefebvr on 15/11/19.
//

#include <cmath>
#include <iostream>
#include "MostInfeasibleBranchingRule.h"

L::Variable L::MostInfeasibleBranchingRule::operator()(L::BranchAndBoundNode & node) {
    float max = std::numeric_limits<float>::lowest();

    Variable* branching_variable = nullptr;
    for (const auto& m : node.solution()) {
        if (m.priority() == 0) continue;
        float min_gap = fmin(m.value() - m.lb(), m.ub() - m.value());
        if (min_gap > max) {
            max = min_gap;
            delete branching_variable;
            branching_variable = new Variable(m);
        }
    }

    if (!branching_variable) throw Exception("Could not determine variable to branch on.");
    Variable output = *branching_variable;
    delete branching_variable;

    return output;
}
