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
    typedef Vector<Constraint> ConstraintVector;
}

/**
 * \brief Implements a vector of components.
 * \details Vectors are a sort of factory for its components. It does not manage their storage.
 * For instance, instead of creating several variables x_1, x_2, x_3, one shall create a VariableVector and ask for x(1), x(2)
 * and x(3). The vector does nothing but calling the Variable's constructor with incrementing variables names.
 * @tparam Component the component types
 */
template<class Component>
class L::Vector {
    Environment& _env; //!< environment holding the created variables
    const std::string _user_defined_name; //!< vector's name (to which will be appended "_{index}" for its components name)
    const unsigned long int _dimension; //!< vector's dimension
    std::map<std::string, Component*> _components; //!< pointers to its components (yet the memory is managed by _env)
protected:
    virtual void after_new_component_hook(Component& component);
public:
    typedef MapIterator<std::string, Component> ComponentIterator;
    ~Vector();

    /**
     * \brief Constructor.
     * @param env environment for components creation
     * @param user_defined_name vector's name
     * @param dimension vector's dimension
     */
    Vector(Environment& env, std::string  user_defined_name, unsigned long int dimension = 1);

    /**
     * \brief Accesses a component if it exists, creates one if not.
     * @param i variadic argument defining the component's index
     * @return corresponding component or new component
     */
    Component operator()(unsigned long int i, ...);

    /**
     * Returns an iterator over the vector's components
     * @return iterator over vector's components
     */
    ComponentIterator components();
};

#include "Vector.cpp"

#endif //LBDS_SOLVER_VECTOR_H
