//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_ABSTRACTVARIABLE_H
#define LSOLVERPROJECT_ABSTRACTVARIABLE_H

#include "../enums.h"

namespace L {
    class AbstractVariable;
    std::ostream& operator<<(std::ostream& os, const AbstractVariable& var);
}

/**
 * \brief Implements the interface for variables
 * \details Variables may be positive, negative, free, integer (assumed positive integer) or binary. This property
 * is referred to as a variable's "type". A variable also posesses a lower bound, upper bound and a value.
 * For linear problems, variables also have a reduced cost (see Simplex). Finally, variables have priority for branching.
 * By default, it is set to zero for continuous variables and to one for integer/binary variables.
 * See the Modeling Objects page for implementation details and its relation with Constraint, CoreConstraint and DetachedConstraint.
 */
class L::AbstractVariable {
public:

    virtual ~AbstractVariable() = default;

    // getters
    [[nodiscard]] virtual float value() const = 0; //!< access variable's value
    [[nodiscard]] virtual float ub() const = 0; //!< access variable's upper bound
    [[nodiscard]] virtual float lb() const = 0; //!< access variable's lower bound
    [[nodiscard]] virtual float reduced_cost() const = 0;//!< access variable's reduced cost (when appropriate)
    [[nodiscard]] virtual VariableType type() const = 0; //!< access variable's type
    [[nodiscard]] virtual unsigned int priority() const = 0;//!< access variable's priority for branching
    [[nodiscard]] virtual const std::string& user_defined_name() const = 0;//! access variable's name

    // setters
    virtual void value(float) = 0; //!< updates variable's value
    virtual void ub(float) = 0; //!< updates variable's upper bound
    virtual void lb(float) = 0; //!< updates variable's lower bound
    virtual void reduced_cost(float) = 0; //!< updates variable's reduced cost
    virtual void type(VariableType) = 0; //!< updates variable's type
    virtual void priority(unsigned int) = 0; //!< update variable's priority for branching
    virtual void force_value_to(float value) = 0;

    friend std::ostream& operator<<(std::ostream& os, const AbstractVariable& var);
};


#endif //LSOLVERPROJECT_ABSTRACTVARIABLE_H
