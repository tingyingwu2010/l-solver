//
// Created by hlefebvr on 05/11/19.
//

#include "Constraint.h"

#include <utility>
#include "../utils/Exception.h"
#include "../environment/Environment.h"

using namespace L;

/** CORE CONSTRAINTS **/

CoreConstraint::CoreConstraint(std::string  user_defined_name) : _user_defined_name(std::move(user_defined_name)) {}

Variable CoreConstraint::dual() {
    if (!_dual_variable) _dual_variable = new CoreVariable("_dual_" + _user_defined_name);
    return Variable(*_dual_variable);
}

ConstConstraint::ConstConstraint(CoreConstraint &core) : _core(core) {}

std::string AbstractConstraint::to_string(CoreConstraint::Type type) {
    switch (type) {
        case LessOrEqualTo: return "<=";
        case GreaterOrEqualTo: return ">=";
        case EqualTo: return "==";
        default: throw Exception("Unknown constraint type");
    }
}

/** CONSTRAINTS **/

Constraint::Constraint(CoreConstraint &core) : _core(core) {}
Constraint::Constraint(Environment & env, const std::string& name) : Constraint(env.constraint(name)) {}

std::ostream &operator<<(std::ostream &os, const Constraint &constraint) {
    os << std::string("Constraint ") << constraint.user_defined_name();
    os << std::string(" ( ") << std::to_string(constraint.status()) << std::string(")\n");
    // os << constraint.expression().to_string() << std::string(" ") << Constraint::to_string(constraint.type()) << std::string(" 0"); // TODO
    return os;
}
