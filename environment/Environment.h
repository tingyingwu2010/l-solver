//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_ENVIRONMENT_H
#define ED_SOLVER_ENVIRONMENT_H

#include "../structures/List.h"
#include "../structures/Map.h"
#include "AbstractEnvironmentVariable.h"
#include "../modeling/VariableVector.h"


namespace ED {

    class Environment : public AbstractEnvironmentVariable {
        VariablePtrList _variables;
        MapVariableVector _variable_vectors;
    public:
        Variable& add_isolated_variable(const std::string& name) override;
        VariableVector& add_variable_vector(const std::string& name) override;
    };

}


#endif //ED_SOLVER_ENVIRONMENT_H
