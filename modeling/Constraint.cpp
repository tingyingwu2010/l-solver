//
// Created by hlefebvr on 05/11/19.
//

#include "Constraint.h"
#include "../utils/Exception.h"

ED::Constraint::Constraint(ED::AbstractEnvironmentConstraint &env, const std::string &user_defined_name)
    : _user_defined_name(user_defined_name) {
    _parent = &env.add_isolated_constraint(user_defined_name);
}

ED::Constraint::Constraint(const std::string& user_defined_name) : _user_defined_name(user_defined_name) {}

std::ostream &ED::operator<<(std::ostream &os, const ED::Constraint &constraint) {
    os << std::string("Constraint ") << constraint._user_defined_name;
    os << std::string((constraint._parent == nullptr) ? " (core)" : " (tmp)") << std::string(": ");
    os << constraint._expr.to_string() << std::string(" ") << Constraint::to_string(constraint._type) << std::string(" 0");
    return os;
}

std::string ED::Constraint::to_string(ED::Constraint::Type type) {
    switch (type) {
        case LessOrEqualTo: return "<=";
        case GreaterOrEqualTo: return ">=";
        case EqualTo: return "==";
        default: throw Exception("Unknown constraint type");
    }
}
