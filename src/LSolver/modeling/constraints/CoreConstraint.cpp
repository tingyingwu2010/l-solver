//
// Created by hlefebvr on 20/11/19.
//

#include "CoreConstraint.h"
#include "../variables/Variable.h"

L::CoreConstraint::CoreConstraint(std::string  user_defined_name) : _user_defined_name(std::move(user_defined_name)) {}

L::Variable L::CoreConstraint::dual() {
    if (!_dual_variable) {
        _dual_variable = new CoreVariable("_dual_" + _user_defined_name);
        if (_type == LessOrEqualTo)  _dual_variable->type(Negative);
        else if (_type == EqualTo) _dual_variable->type(Free);
    }
    return Variable(*_dual_variable);
}

L::CoreConstraint::~CoreConstraint() {
    delete _dual_variable;
}
