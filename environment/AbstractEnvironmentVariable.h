//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_ABSTRACTENVIRONMENTVARIABLE_H
#define ED_SOLVER_ABSTRACTENVIRONMENTVARIABLE_H

#include <string>

namespace L {
    class Variable;
    class VariableVector;
    /**
     * Interface for the Variable class to speak with Environment
     */
    class AbstractEnvironmentVariable {
    public:
        virtual Variable& add_isolated_variable(const std::string& name) = 0;
        virtual VariableVector& add_variable_vector(const std::string& name) = 0;
    };
}


#endif //ED_SOLVER_ABSTRACTENVIRONMENTVARIABLE_H
