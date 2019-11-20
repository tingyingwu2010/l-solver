//
// Created by hlefebvr on 14/11/19.
//

#ifndef LBDS_SOLVER_VECTOR_H
#define LBDS_SOLVER_VECTOR_H

#include <utility>
#include <map>
#include <cstdarg>
#include "LSolver/modeling/variables/Variable.h"
#include "LSolver/modeling/constraints/Constraint.h"
#include "LSolver/structures/MapIterator.h"

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
    ComponentIterator components();
};

#include "Vector.cpp"

#endif //LBDS_SOLVER_VECTOR_H
