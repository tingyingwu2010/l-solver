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
    if (!_dual_variable) {
        _dual_variable = new CoreVariable("_dual_" + _user_defined_name);
        if (_type == LessOrEqualTo)  _dual_variable->type(Negative);
        else if (_type == EqualTo) _dual_variable->type(Free);
    }
    return Variable(*_dual_variable);
}

ConstConstraint::ConstConstraint(CoreConstraint &core) : _core(core) {}

std::string AbstractConstraint::to_string(ConstraintType type) {
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

std::ostream &L::operator<<(std::ostream &os, const L::AbstractConstraint &constraint) {
    os << std::string("(Constraint) ") << constraint.expression().to_string() << std::string(" ") << L::Constraint::to_string(constraint.type()) << std::string(" 0");
    return os;
}

std::ostream& L::operator<<(std::ostream& os, ConstraintType constraint) {
    switch (constraint) {
        case LessOrEqualTo: return (os << std::string("<="));
        case GreaterOrEqualTo: return (os << std::string(">="));
        case EqualTo: return (os << std::string("=="));
        default: throw Exception("Unknown constraint type: " + std::to_string(constraint));
    }
}

DetachedConstraint::DetachedConstraint(const Constraint &src, bool detach_dual)
    : CoreConstraint(src.user_defined_name()),
        _core(src._core),
        _detach_dual(detach_dual) {
    _type = _core.type();
    _expr = _core.expression();
    _slack = _core.slack();
}

Variable DetachedConstraint::dual() {
    if (_detach_dual) return CoreConstraint::dual();
    return _core.dual();
}

