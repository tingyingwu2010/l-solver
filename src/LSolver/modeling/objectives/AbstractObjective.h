//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_ABSTRACTOBJECTIVE_H
#define LSOLVERPROJECT_ABSTRACTOBJECTIVE_H

#include <iostream>
#include "..//expressions/Expression.h"
#include "../enums.h"

namespace L {
    class AbstractObjective;
    std::ostream& operator<<(std::ostream& os, const AbstractObjective& obj);
}

/**
 * \brief Implements the interface for Objective's.
 * \details Objective typically have an expression (i.e., the function to be optimize), a type (i.e., minimize or maximize),
 * a value (resulting from the last evaluation/solution) and a status (resulting from the last algorithm executed to optimize it, see ObjectiveStatus)
 * See the Modeling Objects page for more details and its relation with CoreObjective, Objective and DetachedObjective.
 */
class L::AbstractObjective {
public:
    /**
     * \brief Returns the objective's type
     * @return objective's type
     */
    [[nodiscard]] virtual ObjectiveType type() const =  0;

    /**
     * \brief Return the objective's name
     * @return objective's name
     */
    [[nodiscard]] virtual const std::string& user_defined_name() const = 0;

    /**
     * \brief Returns the objective status
     * @return objective's status
     */
    [[nodiscard]] virtual ObjectiveStatus status() const = 0;

    /**
     * \brief Returns the objective's value
     * @return objective's value
     */
    [[nodiscard]] virtual float value() const = 0;

    /**
     * \brief Returns the objective's expression
     * @return objective's expression
     */
    [[nodiscard]] virtual const Expression& expression() const = 0;

    /**
     * \brief Returns the objective's expression as a reference so it can be modified.
     * @return objective's expression
     */
    virtual Expression& expression() = 0;

    /**
     * \brief Changes the objective's type
     * @param type new objective's type
     */
    virtual void type(ObjectiveType type) = 0;

    /**
     * \brief Changes the objective's status
     * @param status new objective's status
     */
    virtual void status(ObjectiveStatus status) = 0;

    /**
     * \brief Changes the objective's value
     * @param value new objective's value
     */
    virtual void value(float value) = 0;

    friend std::ostream& operator<<(std::ostream& os, const AbstractObjective& obj);
};


#endif //LSOLVERPROJECT_ABSTRACTOBJECTIVE_H
