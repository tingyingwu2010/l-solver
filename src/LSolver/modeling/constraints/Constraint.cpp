//
// Created by hlefebvr on 05/11/19.
//

#include "Constraint.h"

#include <utility>
#include "LSolver/utils/Exception.h"
#include "LSolver/environment/Environment.h"

using namespace L;

ConstConstraint::ConstConstraint(CoreConstraint &core) : _core(core) {}

std::string AbstractConstraint::to_string(ConstraintType type) {
    switch (type) {
        case LessOrEqualTo: return "<=";
        case GreaterOrEqualTo: return ">=";
        case EqualTo: return "==";
        default: throw Exception("Unknown constraints type");
    }
}

/** CONSTRAINTS **/

Constraint::Constraint(CoreConstraint &core) : _core(core) {}
Constraint::Constraint(Environment & env, const std::string& name) : Constraint(env.constraint(name)) {}


