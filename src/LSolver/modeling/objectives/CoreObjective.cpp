//
// Created by hlefebvr on 20/11/19.
//

#include "CoreObjective.h"

L::ObjectiveType L::CoreObjective::type() const {
    return _type;
}

const std::string &L::CoreObjective::user_defined_name() const {
    return _user_defined_name;
}

const L::Expression &L::CoreObjective::expression() const {
    return _expression;
}

L::Expression &L::CoreObjective::expression() {
    return _expression;
}

L::ObjectiveStatus L::CoreObjective::status() const {
    return _status;
}

float L::CoreObjective::value() const {
    return _value;
}

void L::CoreObjective::type(L::ObjectiveType type) {
    if (type == Minimize && _type == Maximize && _value == std::numeric_limits<float>::lowest() && _status == Unsolved)
        _value = std::numeric_limits<float>::max();
    else if (type == Maximize && _type == Minimize && _value == std::numeric_limits<float>::max()  && _status == Unsolved)
        _value = std::numeric_limits<float>::lowest();
    _type = type;
}

void L::CoreObjective::status(L::ObjectiveStatus status) {
    _status = status;
}

void L::CoreObjective::value(float value) {
    _value = value;
}

L::CoreObjective::CoreObjective(const std::string &user_defined_name) : _user_defined_name(user_defined_name) {}

float L::CoreObjective::ub() const {
    return _ub;
}

float L::CoreObjective::lb() const {
    return _lb;
}

void L::CoreObjective::lb(float lb) {
    _lb = lb;
}

void L::CoreObjective::ub(float ub) {
    _ub = ub;
}
