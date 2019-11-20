//
// Created by hlefebvr on 20/11/19.
//

#include <LSolver/utils/Exception.h>
#include <LSolver/application/Application.h>
#include "CoreVariable.h"

L::CoreVariable::CoreVariable(std::string user_defined_name) : _user_defined_name(std::move(user_defined_name)) {}

void L::CoreVariable::type(VariableType type) {
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

void L::CoreVariable::value(float value) {
    float epsilon = Application::parameters().tolerance();
    if (!(_lb - epsilon <= value && value <= _ub + epsilon))
        throw Exception("Value for " + _user_defined_name + " is out of bound: " + std::to_string(value) + " is not in [" + std::to_string(_lb) + ", " + std::to_string(_ub));
    if (_lb > value) _value = _lb;
    else if (_ub < value) _value = _ub;
    else _value = value;
}

void L::CoreVariable::force_value_to(float value) {
    _value = value;
}

void L::CoreVariable::ub(float ub) {
    if (ub < _lb) throw Exception("Trying to set contradictory bounds");
    _ub = ub;
}

void L::CoreVariable::lb(float lb) {
    if (lb > _ub) throw Exception("Trying to set contradictory bounds");
    _lb = lb;
}