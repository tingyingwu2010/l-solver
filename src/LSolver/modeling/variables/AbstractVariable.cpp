//
// Created by hlefebvr on 20/11/19.
//

#include "AbstractVariable.h"

std::ostream &L::operator<<(std::ostream &os, const AbstractVariable &var) {
    os << "(Variable) " << var.lb() << " <= " << var.user_defined_name() << " <= " << var.ub() << ", " << var.type();
    return os;
}