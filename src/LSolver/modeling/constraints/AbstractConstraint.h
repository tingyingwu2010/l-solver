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

/**
 * \brief Implements the interface for constraints
 * \details Constraints are represented as the following: \f{align*} \varphi_1(x) \le 0 \\ \varphi_2(x) \ge 0 \\ \varphi_3(x) = 0  \f}
 * The expression $$\varphi(x)$$ is referred to as the constraint's expression. The constraint's type denotes its "sign" (<=, >=, or ==).
 * See the Modeling Objects page for implementation details and its relation with Constraint, CoreConstraint and DetachedConstraint.
 */
class L::AbstractConstraint {
public:
    /**
     * \brief Returns the constraint type (see ConstraintType)
     * @return constraint's type
     */
    [[nodiscard]] virtual ConstraintType type() const = 0;

    /**
     * \brief Returns the value of the associated slack variable (only for LPs)
     * @return constraint's slack value
     */
    [[nodiscard]] virtual float slack() const = 0;

    /**
     * \brief Returns a const reference to the mathematical left handside expression of the constraint
     * @return Constraint's left handside
     */
    [[nodiscard]] virtual const Expression& expression() const = 0;

    /**
     * \brief Returns the name of the constraint
     * @return Constraint's name
     */
    [[nodiscard]] virtual const std::string& user_defined_name() const = 0;

    /**
     * \brief Returns the dual variable associated to the constraint. Note that no dual variable is actually created
     * unless a first call is made to this function.
     * @return Constraint's dual variable
     */
    virtual Variable dual() = 0;

    /**
     * \brief Returns a reference to the mathematical left handside expression of the constraint
     * @return Constraint's left handside
     */
    virtual Expression& expression() = 0;

    /**
     * \brief Changes the constraint's type
     * @param type new constraint's type
     */
    virtual void type(ConstraintType type) = 0;

    /**
     * \brief Changes the slack variable's value
     * @param s new constraint's slack's value
     */
    virtual void slack(float s) = 0;

    static std::string to_string(ConstraintType type);
};

#endif //LSOLVERPROJECT_ABSTRACTCONSTRAINT_H
