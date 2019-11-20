//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_ENUMS_H
#define LSOLVERPROJECT_ENUMS_H

#include <iostream>

namespace L {
    enum ConstraintType {LessOrEqualTo, GreaterOrEqualTo, EqualTo};
    std::ostream& operator<<(std::ostream& os, ConstraintType constraint);

    enum ObjectiveType { Minimize, Maximize };
    std::ostream& operator<<(std::ostream& os, ObjectiveType type);

    enum ObjectiveStatus { Unsolved, Optimal, Feasible, Infeasible, Unbounded, Error };
    std::ostream& operator<<(std::ostream& os, ObjectiveStatus obj);

    enum VariableType { Positive, Negative, Free, Binary, Integer };
    std::ostream &operator<<(std::ostream &os, VariableType type);
}

#endif //LSOLVERPROJECT_ENUMS_H
