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

class L::AbstractVariable {
public:
    enum Status { Core, Default, Detached };

    virtual ~AbstractVariable() = default;

    // getters
    [[nodiscard]] virtual float value() const = 0; //!< access variable's value
    [[nodiscard]] virtual float ub() const = 0; //!< access variable's upper bound
    [[nodiscard]] virtual float lb() const = 0; //!< access variable's lower bound
    [[nodiscard]] virtual float reduced_cost() const = 0;//!< access variable's reduced cost (when appropriate)
    [[nodiscard]] virtual VariableType type() const = 0; //!< access variable's type
    [[nodiscard]] virtual Status status() const = 0;//!< access variables's status
    [[nodiscard]] virtual unsigned int priority() const = 0;//!< access variable's priority for branching
    [[nodiscard]] virtual const std::string& user_defined_name() const = 0;

    // setters
    virtual void value(float) = 0; //!< updates variable's value
    virtual void ub(float) = 0; //!< updates variable's upper bound
    virtual void lb(float) = 0; //!< updates variable's lower bound
    virtual void reduced_cost(float) = 0; //!< updates variable's reduced cost
    virtual void type(VariableType) = 0; //!< updates variable's type
    virtual void priority(unsigned int) = 0; //<! update variable's priority for branching
    virtual void force_value_to(float value) = 0;

    friend std::ostream& operator<<(std::ostream& os, const AbstractVariable& var);
};


#endif //LSOLVERPROJECT_ABSTRACTVARIABLE_H
