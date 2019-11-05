//
// Created by hlefebvr on 05/11/19.
//

#ifndef ED_SOLVER_CONSTRAINTVECTOR_H
#define ED_SOLVER_CONSTRAINTVECTOR_H

#include "../modeling/Constraint.h"
#include "../structures/Vector.h"
#include "../environment/AbstractEnvironmentConstraint.h"

namespace ED {

    class ConstraintVector : public AbstractConstraintVector {
        friend class Environment;
        ConstraintVector* _parent = nullptr;
        const std::string _user_defined_name;
        Constraint& on_component_not_found(const VectorIndex& index) override;
        explicit ConstraintVector(const std::string& name);
    public:
        explicit ConstraintVector(AbstractEnvironmentConstraint& env, const std::string& name);
        virtual ~ConstraintVector();
        const ConstraintVector& core() const { return *_parent; } //!< returns the core vector attached to the environment

        friend std::ostream& operator<<(std::ostream& os, const ConstraintVector& vec);
    };

}


#endif //ED_SOLVER_CONSTRAINTVECTOR_H
