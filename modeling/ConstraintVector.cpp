//
// Created by hlefebvr on 05/11/19.
//

#include <iostream>
#include "Constraint.h"
#include "ConstraintVector.h"
#include "../utils/static.h"

ED::ConstraintVector::ConstraintVector(ED::AbstractEnvironmentConstraint &env, const std::string &name) : _user_defined_name(name) {
    _parent = &env.add_constraint_vector(name);
}

ED::ConstraintVector::ConstraintVector(const std::string &name) : _user_defined_name(name) {}

ED::Constraint &ED::ConstraintVector::on_component_not_found(const ED::VectorIndex &index) {
    if (_parent != nullptr) {
        auto& new_core_component = _parent->on_component_not_found(index);
        auto& new_tmp_component = *new Constraint(new_core_component._user_defined_name);
        new_tmp_component._parent = &new_core_component;
        add_component(index, new_tmp_component);
        return new_tmp_component;
    }

    auto& new_core_component = *new Constraint(concat_string_array(_user_defined_name, index));
    add_component(index, new_core_component);
    return new_core_component;

}

std::ostream &ED::operator<<(std::ostream &os, const ConstraintVector &vec) {
    os << std::string("Constraint ") << vec._user_defined_name;
    os << std::string((vec._parent == nullptr) ? " (core)" : " (tmp)") << std::endl;
    for (auto m : vec._components)
        os << *m.second << std::endl;
    return os;
}

ED::ConstraintVector::~ConstraintVector() {
    for (auto m : _components) delete m.second;
}
