//
// Created by hlefebvr on 05/11/19.
//

#include "Constraint.h"

#include <utility>
#include "../utils/Exception.h"

ED::CoreConstraint::CoreConstraint(std::string  user_defined_name) : _user_defined_name(std::move(user_defined_name)) {}

std::ostream &ED::operator<<(std::ostream &os, const ED::Constraint &constraint) {
    os << std::string("Constraint ") << constraint.user_defined_name();
    os << std::string(" ( ") << std::to_string(constraint.status()) << std::string(")\n");
    os << constraint.expression().to_string() << std::string(" ") << Constraint::to_string(constraint.type()) << std::string(" 0");
    return os;
}

std::string ED::AbstractConstraint::to_string(ED::CoreConstraint::Type type) {
    switch (type) {
        case LessOrEqualTo: return "<=";
        case GreaterOrEqualTo: return ">=";
        case EqualTo: return "==";
        default: throw Exception("Unknown constraint type");
    }
}

ED::Constraint::Constraint(ED::CoreConstraint &core) : _core(core) {}

ED::ConstConstraint::ConstConstraint(ED::CoreConstraint &core) : _core(core) {}
