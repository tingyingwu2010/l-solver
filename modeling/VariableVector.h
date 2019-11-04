//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_VARIABLEVECTOR_H
#define ED_SOLVER_VARIABLEVECTOR_H

#include "../modeling/Variable.h"
#include "../structures/Vector.h"
#include "../environment/AbstractEnvironmentVariable.h"

namespace ED {

    class VariableVector : public AbstractVariableVector {
        friend class Environment;
        VariableVector* _parent = nullptr;
        const std::string _user_defined_name;
        Variable& on_component_not_found(const VectorIndex& index) override;
        explicit VariableVector(const std::string& name);
    public:
        explicit VariableVector(AbstractEnvironmentVariable& env, const std::string& name);
        virtual ~VariableVector();
        const VariableVector& core() const { return *_parent; } //!< returns the core vector attached to the environment

        friend std::ostream& operator<<(std::ostream& os, const VariableVector& vec);
    };

}

#endif //ED_SOLVER_VARIABLEVECTOR_H
