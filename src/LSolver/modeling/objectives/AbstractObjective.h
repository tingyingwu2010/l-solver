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

class L::AbstractObjective {
public:
    // getters
    [[nodiscard]] virtual ObjectiveType type() const =  0;
    [[nodiscard]] virtual const std::string& user_defined_name() const = 0;
    [[nodiscard]] virtual ObjectiveStatus status() const = 0;
    [[nodiscard]] virtual float value() const = 0;
    [[nodiscard]] virtual const Expression& expression() const = 0;
    virtual Expression& expression() = 0;

    // setters
    virtual void type(ObjectiveType) = 0;
    virtual void status(ObjectiveStatus) = 0;
    virtual void value(float) = 0;

    friend std::ostream& operator<<(std::ostream& os, const AbstractObjective& obj);
};


#endif //LSOLVERPROJECT_ABSTRACTOBJECTIVE_H
