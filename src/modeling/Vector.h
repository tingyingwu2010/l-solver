//
// Created by hlefebvr on 14/11/19.
//

#ifndef LBDS_SOLVER_VECTOR_H
#define LBDS_SOLVER_VECTOR_H

#include <utility>
#include <map>
#include <cstdarg>
#include "Variable.h"
#include "Constraint.h"
#include "../structures/MapIterator.h"

namespace L {
    class Environment;
    template<class Component> class Vector;
    typedef Vector<Variable> VariableVector;
    typedef Vector<Constraint> ConstraintVector;
}

template<class Component>
class L::Vector {
    Environment& _env;
    const std::string _user_defined_name;
    const unsigned long int _dimension;
    std::map<std::string, Component*> _components;
public:
    typedef MapIterator<std::string, Component> ComponentIterator;
    ~Vector();
    Vector(Environment& env, std::string  user_defined_name, unsigned long int dimension = 1);
    Component operator()(unsigned long int i, ...);
    ComponentIterator components() const;
};

template<class Component>
L::Vector<Component>::Vector(L::Environment &env, std::string user_defined_name, unsigned long int dimension)
    : _env(env), _user_defined_name(std::move(user_defined_name)), _dimension(dimension) {}

template<class Component>
Component L::Vector<Component>::operator()(unsigned long int i, ...) {

    va_list raw_indices;
    std::string index = std::to_string(i);
    va_start(raw_indices, i);
    for (unsigned long int k = 1; k < _dimension; k += 1)
        index += "," + std::to_string(va_arg(raw_indices, unsigned long int));
    va_end(raw_indices);

    std::string name = _user_defined_name + "_" + index;
    auto found = _components.find(name);

    if (found == _components.end()) {
        auto* component = new Component(_env, name);
        _components.insert({ name, component });
        return *component;
    }

    return *found->second;
}

template<class Component>
L::Vector<Component>::~Vector() {
    for (auto& m : _components) delete m.second;
}

template<class Component>
typename L::Vector<Component>::ComponentIterator L::Vector<Component>::components() const {
    return ComponentIterator(_components);
}

#endif //LBDS_SOLVER_VECTOR_H
