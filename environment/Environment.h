//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_ENVIRONMENT_H
#define ED_SOLVER_ENVIRONMENT_H

#include "../structures/List.h"
#include "../structures/Map.h"
#include "AbstractEnvironmentVariable.h"
#include "../modeling/VariableVector.h"
#include "AbstractEnvironmentConstraint.h"
#include "../modeling/Constraint.h"


namespace ED {

    class Environment : public AbstractEnvironmentVariable, public AbstractEnvironmentConstraint {
        VariablePtrList _variables;
        MapVariableVector _variable_vectors;
        ConstraintPtrList _constraints;
        MapConstraintVector _constraint_vectors;
    public:
        Variable& add_isolated_variable(const std::string& name) override;
        VariableVector& add_variable_vector(const std::string& name) override;
        Constraint& add_isolated_constraint(const std::string& name) override;
        ConstraintVector& add_constraint_vector(const std::string& name) override;
    };

}


#endif //ED_SOLVER_ENVIRONMENT_H
