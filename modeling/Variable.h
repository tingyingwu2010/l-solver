//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_VARIABLE_H
#define ED_SOLVER_VARIABLE_H

#include <string>
#include "../environment/AbstractEnvironmentVariable.h"

namespace ED {
    /**
     * \brief Represents a decisional variable as used in an optimization model. It is linked to its parent variable stored
     * in the environment.
     */
    class Variable {
        friend class Environment;
        friend class VariableVector;
        enum Type { Positive, Negative, Free, Binary, Integer };
        Variable* _parent = nullptr; //!< parent variable in the environment
        float _value = 0.0; //!< current value
        float _ub = 0.0; //!< lower bound
        float _lb = 0.0; //!< upper bound
        float _reduced_cost = 0.0; //!< reduced cost
        Type _type = Positive; //!< variable's type
        const std::string _user_defined_name;
        explicit Variable(const std::string& name);
    public:
        explicit Variable(AbstractEnvironmentVariable& env, const std::string& name);

        float value() const { return _value; }
        float ub() const { return _ub; }
        float lb() const { return _lb; }
        float reduced_cost() const { return _reduced_cost; }
        Type type() const { return _type; }
        const std::string& user_defined_name() const { return _user_defined_name; }

        void value(float v) { _value = v; }
        void ub(float bound) { _ub = bound; }
        void lb(float bound) { _lb = bound; }
        void type(Type t) { _type = t; }
        void reduced_cost(float r) { _reduced_cost = r; }

        const Variable& core() const { return *_parent; } //!< returns the core variable attached to the environment

        friend std::ostream& operator<<(std::ostream& os, const Variable& var);
    };

};

#endif //ED_SOLVER_VARIABLE_H
