//
// Created by hlefebvr on 04/11/19.
//

#include "Variable.h"
#include "../environment/Environment.h"
#include <iostream>
#include <utility>

using namespace L;

CoreVariable::CoreVariable(std::string user_defined_name) : _user_defined_name(std::move(user_defined_name)) {}

void CoreVariable::type(VariableType type) {
    if (type == Binary) {
        _lb = 0;
        _ub = 1;
        _priority = 1;
    }
    _type = type;
}

std::ostream &L::operator<<(std::ostream &os, const AbstractVariable &var) {
    os << "(Variable) " << var.lb() << " <= " << var.user_defined_name() << " <= " << var.ub() << ", " << var.type();
    return os;
}

std::ostream &L::operator<<(std::ostream &os, VariableType type) {
    switch (type) {
        case Positive: return (os << "positive");
        case Negative: return (os << "negative");
        case Free: return (os << "free");
        case Binary: return (os << "binary");
        case Integer: return (os << "integer");
        default: throw Exception("Unknown variable type: " + std::to_string(type));
    }
}

Variable::Variable(CoreVariable& core) : _core(core) {}
Variable::Variable(Environment& env, const std::string& name) : Variable(env.variable(name)) {}

ConstVariable::ConstVariable(const CoreVariable &core) : _core(core) {}

DetachedVariable::DetachedVariable(const Variable &src) : CoreVariable(src._core), _core(src._core) {}

void DetachedVariable::update_core_value() {
    _core.value(_value);
}
