//
// Created by hlefebvr on 14/11/19.
//

#include "Objective.h"
#include "LSolver/environment/Environment.h"
#include "LSolver/modeling/objectives/CoreObjective.h"

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

