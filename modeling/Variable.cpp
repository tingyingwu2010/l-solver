//
// Created by hlefebvr on 04/11/19.
//

#include "Variable.h"
#include <utility>

CoreVariable::CoreVariable(std::string user_defined_name) : _user_defined_name(std::move(user_defined_name)) {}

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

DetachedVariable::DetachedVariable(Variable &variable) : Variable(variable._core) {}

ConstVariable::ConstVariable(const CoreVariable &core) : _core(core) {}
