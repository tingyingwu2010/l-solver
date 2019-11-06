//
// Created by hlefebvr on 04/11/19.
//

#include "Variable.h"
#include <utility>

ED::CoreVariable::CoreVariable(std::string user_defined_name) : _user_defined_name(std::move(user_defined_name)) {}

std::ostream &ED::operator<<(std::ostream &os, const ED::AbstractVariable &var) {
    os << std::string("Variable ") << var.user_defined_name();
    os << std::string(" (") << std::to_string(var.status()) << std::string(" )");
    os << std::string("\n\tValue : ") << std::to_string(var.value());
    os << std::string("\n\tLB    : ") << std::to_string(var.lb());
    os << std::string("\n\tUB    : ") << std::to_string(var.ub());
    os << std::string("\n\tType  : ") << std::to_string(var.type());
    os << std::string("\n\tR.ct. : ") << std::to_string(var.reduced_cost());
    return os;
}

ED::Variable::Variable(CoreVariable& core) : _core(core) {}

ED::DetachedVariable::DetachedVariable(ED::Variable &variable) : Variable(variable._core) {}

ED::ConstVariable::ConstVariable(const ED::CoreVariable &core) : _core(core) {}
