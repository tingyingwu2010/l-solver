//
// Created by hlefebvr on 20/11/19.
//

#include "AbstractObjective.h"

std::ostream &L::operator<<(std::ostream &os, const L::AbstractObjective &obj) {
    os << std::string("(Objective) ") << obj.type() << std::string(" ") << obj.expression();
    return os;
}