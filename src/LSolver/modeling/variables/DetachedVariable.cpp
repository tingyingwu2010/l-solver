//
// Created by hlefebvr on 20/11/19.
//

#include "DetachedVariable.h"
#include "Variable.h"

L::DetachedVariable::DetachedVariable(const Variable &src) : CoreVariable(src._core), _core(src._core) {}

void L::DetachedVariable::update_core_value() {
    _core.value(_value);
}