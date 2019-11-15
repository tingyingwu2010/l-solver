//
// Created by hlefebvr on 04/11/19.
//

#include "Variable.h"
#include "../environment/Environment.h"
#include <utility>

using namespace L;

CoreVariable::CoreVariable(std::string user_defined_name) : _user_defined_name(std::move(user_defined_name)) {}

void CoreVariable::type(AbstractVariable::Type type) {
    // TODO change bounds
    _type = type;
}

std::ostream &operator<<(std::ostream &os, const AbstractVariable &var) {
    os << std::string("Var ") << var.user_defined_name();
    os << std::string(" (") << std::to_string(var.status()) << std::string(" )");
    os << std::string("\n\tValue : ") << std::to_string(var.value());
    os << std::string("\n\tLB    : ") << std::to_string(var.lb());
    os << std::string("\n\tUB    : ") << std::to_string(var.ub());
    os << std::string("\n\tType  : ") << std::to_string(var.type());
    os << std::string("\n\tR.ct. : ") << std::to_string(var.reduced_cost());
    return os;
}

Variable::Variable(CoreVariable& core) : _core(core) {}
Variable::Variable(Environment& env, const std::string& name) : Variable(env.variable(name)) {}

ConstVariable::ConstVariable(const CoreVariable &core) : _core(core) {}

DetachedVariable::DetachedVariable(const Variable &src) : CoreVariable(src.user_defined_name()), _core(src._core) {}

void DetachedVariable::update_core_value() {
    _core.value(_value);
}
