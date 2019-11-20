//
// Created by hlefebvr on 04/11/19.
//

#include "Variable.h"
#include "../environment/Environment.h"
#include <iostream>
#include <utility>
#include <LSolver/application/Application.h>

using namespace L;

CoreVariable::CoreVariable(std::string user_defined_name) : _user_defined_name(std::move(user_defined_name)) {}

void CoreVariable::type(VariableType type) {
    if (type == Binary) {
        _lb = 0;
        _ub = 1;
        _priority = 1;
    } else {
        _priority = 0;
        if (type == Positive|| type == Integer) {
            _lb = 0;
            _ub = std::numeric_limits<float>::max();
        } else if (type == Negative) {
            _lb = std::numeric_limits<float>::lowest();
            _ub = 0;
        } else if (type == Free) {
            _lb = std::numeric_limits<float>::lowest();
            _ub = std::numeric_limits<float>::max();
        } else {
            throw Exception("Unknown variable type: " + std::to_string(type));
        }
    }

    if (_lb > _ub) throw Exception("Lower bound is greater than upper bound for variable " + _user_defined_name);
    _type = type;
}

void CoreVariable::value(float value) {
    float epsilon = Application::parameters().tolerance();
    if (!(_lb - epsilon <= value && value <= _ub + epsilon))
        throw Exception("Value for " + _user_defined_name + " is out of bound: " + std::to_string(value) + " is not in [" + std::to_string(_lb) + ", " + std::to_string(_ub));
    if (_lb > value) _value = _lb;
    else if (_ub < value) _value = _ub;
    else _value = value;
}

void CoreVariable::force_value_to(float value) {
    _value = value;
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
