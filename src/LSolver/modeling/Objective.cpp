//
// Created by hlefebvr on 14/11/19.
//

#include "Objective.h"
#include "../environment/Environment.h"

std::ostream& L::operator<<(std::ostream& os, ObjectiveType type) {
    switch (type) {
        case Minimize: return (os << std::string("minimize"));
        case Maximize: return (os << std::string("maximize"));
        default: throw Exception("Unknown objective type: " + std::to_string(type));
    }
}

std::ostream& L::operator<<(std::ostream& os, ObjectiveStatus obj) {
    switch (obj) {

        case Unsolved: return (os << std::string("Unsolved"));
        case Optimal: return (os << std::string("Optimal"));
        case Feasible: return (os << std::string("Feasible"));
        case Infeasible: return (os << std::string("Infeasible"));
        case Unbounded: return (os << std::string("Unbounded"));
        case Error:  return (os << std::string("Error"));
        default: throw Exception("Unknown objective status: " + std::to_string(obj));
    }
}

/** CORE OBJECTIVE **/

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

/** OBJECTIVE **/
L::ObjectiveType L::Objective::type() const {
    return _core.type();
}

const std::string &L::Objective::user_defined_name() const {
    return _core.user_defined_name();
}

const L::Expression &L::Objective::expression() const {
    return _core.expression();
}

L::Expression &L::Objective::expression() {
    return _core.expression();
}

L::ObjectiveStatus L::Objective::status() const {
    return _core.status();
}

float L::Objective::value() const {
    return _core.value();
}

void L::Objective::type(L::ObjectiveType type) {
    return _core.type(type);
}

void L::Objective::status(L::ObjectiveStatus status) {
    return _core.status(status);
}

void L::Objective::value(float value) {
    return _core.value(value);
}

L::Objective::Objective(const L::Objective & obj) : _core(obj._core) {}

L::Objective::Objective(L::CoreObjective & core) : _core(core) {}

L::Objective::Objective(L::Environment &env, const std::string &user_defined_name)
    : Objective(env.objective(user_defined_name)) {}

/** CONST OBJECTIVE **/

L::ConstObjective::ConstObjective(L::CoreObjective & core) : _core(core) {}

L::ObjectiveType L::ConstObjective::type() const {
    return _core.type();
}

const std::string &L::ConstObjective::user_defined_name() const {
    return _core.user_defined_name();
}

const L::Expression &L::ConstObjective::expression() const {
    return _core.expression();
}

L::ObjectiveStatus L::ConstObjective::status() const {
    return _core.status();
}

float L::ConstObjective::value() const {
    return _core.value();
}

void L::ConstObjective::type(L::ObjectiveType type) {}

void L::ConstObjective::status(L::ObjectiveStatus status) {}

void L::ConstObjective::value(float value) {}

L::Expression &L::ConstObjective::expression() {
    throw Exception("ConstObjective cannot be modified");
}

std::ostream &L::operator<<(std::ostream &os, const L::AbstractObjective &obj) {
    os << std::string("(Objective) ") << obj.type() << std::string(" ") << obj.expression();
    return os;
}
