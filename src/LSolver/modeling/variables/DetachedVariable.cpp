//
// Created by hlefebvr on 20/11/19.
//

#include "DetachedVariable.h"
#include "Variable.h"

L::DetachedVariable::DetachedVariable(const Variable &src) : CoreVariable(src._core), _core(src._core) {}

void L::DetachedVariable::update_core_value() {
    _core.value(_value);
}

void L::DetachedVariable::update_core_bounds() {
    _core.lb(_lb);
    _core.ub(_ub);
}

void L::DetachedVariable::update_this_bounds() {
    _lb = _core.lb();
    _ub = _core.ub();
}

L::DetachedVariable::DetachedVariable(const L::DetachedVariable &src) : CoreVariable(src._user_defined_name), _core(src._core) {
    _lb = src._lb;
    _ub = src._ub;
    _value = src._value;
    _priority = src._priority;
    _reduced_cost = src._reduced_cost;
    _type = src._type;
}

void L::DetachedVariable::update_this_value() {
    _value = _core.value();
}
