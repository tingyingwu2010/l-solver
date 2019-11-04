//
// Created by hlefebvr on 04/11/19.
//

#include "Variable.h"

ED::Variable::Variable(ED::AbstractEnvironmentVariable &env, const std::string &name) : _user_defined_name(name) {
    _parent = &env.add_isolated_variable(name);
}

ED::Variable::Variable(const std::string &name) : _user_defined_name(name) {}

std::ostream &ED::operator<<(std::ostream &os, const ED::Variable &var) {
    os << std::string("Variable ") << var._user_defined_name;
    os << std::string((var._parent == nullptr) ? " (core)" : " (tmp)");
    os << std::string("\n\tValue : ") << std::to_string(var.value());
    os << std::string("\n\tLB    : ") << std::to_string(var.lb());
    os << std::string("\n\tUB    : ") << std::to_string(var.ub());
    os << std::string("\n\tType  : ") << std::to_string(var.type());
    os << std::string("\n\tR.ct. : ") << std::to_string(var.reduced_cost());
    return os;
}
