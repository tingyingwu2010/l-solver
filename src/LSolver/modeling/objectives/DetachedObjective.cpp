//
// Created by hlefebvr on 20/11/19.
//

#include "DetachedObjective.h"

L::DetachedObjective::DetachedObjective(const L::Objective & src) : CoreObjective(src.user_defined_name()), _core(src._core) {
    _expression = _core.expression();
    _type = _core.type();
    _status = _core.status();
}

void L::DetachedObjective::update_core_value() {
    _core.value(value());
}