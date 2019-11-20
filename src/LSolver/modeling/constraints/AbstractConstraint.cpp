//
// Created by hlefebvr on 20/11/19.
//

#include "AbstractConstraint.h"
#include "../expressions/Expression.h"

std::ostream &L::operator<<(std::ostream &os, const L::AbstractConstraint &constraint) {
    os << std::string("(Constraint) ") << constraint.expression().to_string() << std::string(" ") << constraint.type() << std::string(" 0");
    return os;
}
