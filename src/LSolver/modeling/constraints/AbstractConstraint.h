//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_ABSTRACTCONSTRAINT_H
#define LSOLVERPROJECT_ABSTRACTCONSTRAINT_H

#include "../enums.h"

namespace L {
    class AbstractConstraint;
    class Expression;
    class Variable;

    std::ostream& operator<<(std::ostream& os, const AbstractConstraint& constraint);
}

class L::AbstractConstraint {
public:
    enum Status { Core, Default };

    // getters
    [[nodiscard]] virtual ConstraintType type() const = 0;
    [[nodiscard]] virtual Status status() const = 0;
    [[nodiscard]] virtual float slack() const = 0;
    [[nodiscard]] virtual const Expression& expression() const = 0;
    [[nodiscard]] virtual const std::string& user_defined_name() const = 0;
    virtual Variable dual() = 0;
    virtual Expression& expression() = 0;

    // setters
    virtual void type(ConstraintType) = 0;
    virtual void slack(float s) = 0;

    static std::string to_string(ConstraintType type);
};

#endif //LSOLVERPROJECT_ABSTRACTCONSTRAINT_H
