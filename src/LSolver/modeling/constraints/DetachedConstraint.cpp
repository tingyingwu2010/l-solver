//
// Created by hlefebvr on 20/11/19.
//

#include "DetachedConstraint.h"
#include "../variables/Variable.h"

L::DetachedConstraint::DetachedConstraint(const Constraint &src, bool detach_dual)
        : CoreConstraint(src.user_defined_name()),
          _core(src._core),
          _detach_dual(detach_dual) {
    _type = _core.type();
    _expr = _core.expression();
    _slack = _core.slack();
}

L::Variable L::DetachedConstraint::dual() {
    if (_detach_dual) return CoreConstraint::dual();
    return _core.dual();
}
