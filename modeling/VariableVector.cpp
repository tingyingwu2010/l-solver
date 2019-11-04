//
// Created by hlefebvr on 04/11/19.
//

#include <string>
#include <ostream>
#include "VariableVector.h"
#include "../utils/static.h"

ED::VariableVector::VariableVector(ED::AbstractEnvironmentVariable &env, const std::string &name) : _user_defined_name(name) {
    _parent = &env.add_variable_vector(name);
}

ED::VariableVector::VariableVector(const std::string &name) : _user_defined_name(name) {}

ED::Variable &ED::VariableVector::on_component_not_found(const ED::VectorIndex &index) {
    if (_parent != nullptr) {
        auto& new_core_component = _parent->on_component_not_found(index);
        auto& new_tmp_component = *new Variable(new_core_component._user_defined_name);
        new_tmp_component._parent = &new_core_component;
        add_component(index, new_tmp_component);
        return new_tmp_component;
    }

    auto& new_core_component = *new Variable(concat_string_array(_user_defined_name, index));
    add_component(index, new_core_component);
    return new_core_component;

}

std::ostream &ED::operator<<(std::ostream &os, const VariableVector &vec) {
    os << std::string("Vector ") << vec._user_defined_name;
    os << std::string((vec._parent == nullptr) ? " (core)" : " (tmp)") << std::endl;
    for (auto m : vec._components)
        os << *m.second << std::endl;
    return os;
}