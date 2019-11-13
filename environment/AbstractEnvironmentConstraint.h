//
// Created by hlefebvr on 05/11/19.
//

#ifndef ED_SOLVER_ABSTRACTENVIRONMENTCONSTRAINT_H
#define ED_SOLVER_ABSTRACTENVIRONMENTCONSTRAINT_H

#include <string>

namespace L {
    class Constraint;

    class AbstractEnvironmentConstraint {
    public:
        virtual Constraint& add_isolated_constraint(const std::string& name) = 0;
    };
}

#endif //ED_SOLVER_ABSTRACTENVIRONMENTCONSTRAINT_H
