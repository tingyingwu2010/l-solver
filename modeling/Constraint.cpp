//
// Created by hlefebvr on 05/11/19.
//

#include "Constraint.h"

#include <utility>
#include "../utils/Exception.h"

CoreConstraint::CoreConstraint(std::string  user_defined_name) : _user_defined_name(std::move(user_defined_name)) {}

std::ostream &operator<<(std::ostream &os, const Constraint &constraint) {
    os << std::string("Constraint ") << constraint.user_defined_name();
    os << std::string(" ( ") << std::to_string(constraint.status()) << std::string(")\n");
    // os << constraint.expression().to_string() << std::string(" ") << Constraint::to_string(constraint.type()) << std::string(" 0"); // TODO
    return os;
}

std::string AbstractConstraint::to_string(CoreConstraint::Type type) {
    switch (type) {
        case LessOrEqualTo: return "<=";
        case GreaterOrEqualTo: return ">=";
        case EqualTo: return "==";
        default: throw Exception("Unknown constraint type");
    }
}

Constraint::Constraint(CoreConstraint &core) : _core(core) {}

ConstConstraint::ConstConstraint(CoreConstraint &core) : _core(core) {}
