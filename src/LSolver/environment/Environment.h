//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_ENVIRONMENT_H
#define ED_SOLVER_ENVIRONMENT_H

#include <vector>
#include <map>
#include "../utils/Exception.h"
#include "LSolver/modeling/constraints/Constraint.h"
#include "LSolver/modeling/variables/Variable.h"
#include "LSolver/modeling/objectives/Objective.h"
#include "LSolver/modeling/objectives/CoreObjective.h"

namespace L {
    class Environment;
    template<class, class, class, class> class Set;
}

/**
 * \brief Represents a composition of components. Sets are responsible for the life and death of their components.
 * @tparam Index the class used to represent an index for the components (e.g., storage by name -> std::string)
 * @tparam CoreComponent class which implements the core components (see http://hlefebvr.github.io/posts/en_lsolver_modeling_milp.html)
 * @tparam Component class which implements the regular components (see http://hlefebvr.github.io/posts/en_lsolver_modeling_milp.html)
 * @tparam ConstComponent class which implements the const components (see http://hlefebvr.github.io/posts/en_lsolver_modeling_milp.html)
 */
template <class Index, class CoreComponent, class Component, class ConstComponent>
class L::Set {
    std::map<Index, CoreComponent*> _components; //!< Set of core components associated to their index
    Environment& _env; //!< The Environment to which the set belongs
public:
    class iterator;
    class const_iterator;

    /**
     * \brief Constructor.
     * @param env Environment to which the set belongs
     */
    explicit Set(Environment& env) : _env(env) {}

    /**
     * \brief Destructor. Clears memory. Deletes each of its components.
     */
    virtual ~Set();

    /**
     * \brief Returns a (regular) component (see http://hlefebvr.github.io/posts/en_lsolver_modeling_milp.html) if it exists
     * of creates a new core component and returns a regular component pointing to it  (see http://hlefebvr.github.io/posts/en_lsolver_modeling_milp.html)
     * @param index associated index
     * @return new or existing component
     */
    Component get_or_create(const Index& index);

    /**
     * \brief Returns a (regular) component  (see http://hlefebvr.github.io/posts/en_lsolver_modeling_milp.html) pointing to
     * the core component  (see http://hlefebvr.github.io/posts/en_lsolver_modeling_milp.html) associated with the given index.
     * Throws an exception if no component can be found.
     * @param index associated index
     * @return existing component
     */
    ConstComponent get(const Index& index) const;

    /**
     * \brief Iterator begin.
     * @return iterator.
     */
    iterator begin() { return iterator(_components, false); }

    /**
     * \brief Iterator end.
     * @return iterator.
     */
    iterator end() { return iterator(_components, true); }

    /**
     * \brief Const Iterator begin.
     * @return iterator.
     */
    [[nodiscard]] const_iterator cbegin() const { return const_iterator(_components, false); }

    /**
     * \brief Const Iterator end.
     * @return iterator.
     */
    [[nodiscard]] const_iterator cend() const { return const_iterator(_components, true); }
};

/**
 * \brief Implements a modeling Environment (see http://hlefebvr.github.io/posts/en_lsolver_modeling_milp.html)
 */
class L::Environment  {
public:
    typedef Set<std::string, CoreVariable, Variable, ConstVariable> VariableSet;
    typedef Set<std::string, CoreConstraint, Constraint, ConstConstraint> ConstraintSet;
    typedef Set<std::string, CoreObjective, Objective, ConstObjective> ObjectiveSet;
private:
    VariableSet _variables = VariableSet(*this); //!< set of variables
    ConstraintSet _constraints = ConstraintSet(*this); //!< set of constraints
    ObjectiveSet _objectives = ObjectiveSet(*this); //!< set of objectives
public:
    // Variables
    /**
     * \brief Returns the set of variables belonging to the environment
     * @return set of variables belonging to the environment
     */
    VariableSet& variables() { return _variables; }

    /**
     * \brief Returns the set of variables belonging to the environment
     * @return set of variables belonging to the environment
     */
    [[nodiscard]] const VariableSet& variables() const { return _variables; }

    /**
     * \brief Returns a variable by name or creates a new one and returns
     * @param name name of the variable
     * @return existing or new variable
     */
    Variable variable(const std::string& name) { return _variables.get_or_create(name); }

    /**
     * \brief Returns a variable by name or creates a new one and returns
     * @param name name of the variable
     * @return existing or new variable
     */
    [[nodiscard]] ConstVariable variable(const std::string& name) const { return _variables.get(name); }

    // Constraints
    /**
     * \brief Returns the set of constraints belonging to the environment
     * @return set of constraints belonging to the environment
     */
    ConstraintSet& constraints() { return _constraints; }

    /**
     * \brief Returns the set of constraints belonging to the environment
     * @return set of constraints belonging to the environment
     */
    [[nodiscard]] const ConstraintSet& constraints() const { return _constraints; }

    /**
     * \brief Returns a constraint by name or creates a new one and returns
     * @param name name of the constraint
     * @return existing or new constraint
     */
    Constraint constraint(const std::string& name) { return _constraints.get_or_create(name); }

    /**
     * \brief Returns a constraint by name or creates a new one and returns
     * @param name name of the constraint
     * @return existing or new constraint
     */
    [[nodiscard]] ConstConstraint constraint(const std::string& name) const { return _constraints.get(name); }

    // Objectives
    /**
     * \brief Returns the set of objectives belonging to the environment
     * @return set of objectives belonging to the environment
     */
    ObjectiveSet& objectives() { return _objectives; }

    /**
     * \brief Returns the set of objectives belonging to the environment
     * @return set of objectives belonging to the environment
     */
    [[nodiscard]] const ObjectiveSet& objectives() const { return _objectives; }

    /**
     * \brief Returns a objective by name or creates a new one and returns
     * @param name name of the objective
     * @return existing or new objective
     */
    Objective objective(const std::string& name) { return _objectives.get_or_create(name); }

    /**
     * \brief Returns a objective by name or creates a new one and returns
     * @param name name of the objective
     * @return existing or new objective
     */
    [[nodiscard]] ConstObjective objective(const std::string& name) const { return _objectives.get(name); }
};

template<class Index, class CoreComponent, class Component, class ConstComponent>
L::Set<Index, CoreComponent, Component, ConstComponent>::~Set() {
    for (auto m : _components) delete m.second;
}

template<class Index, class CoreComponent, class Component, class ConstComponent>
Component L::Set<Index, CoreComponent, Component, ConstComponent>::get_or_create(const Index &index) {
    auto found = _components.find(index);
    if (found != _components.end()) return Component(*found->second);
    auto new_component = new CoreComponent(index);
    _components.insert({ index, new_component });
    return Component(*new_component);
}

template<class Index, class CoreComponent, class Component, class ConstComponent>
ConstComponent L::Set<Index, CoreComponent, Component, ConstComponent>::get(const Index &index) const {
    auto found = _components.find(index);
    if (found != _components.end()) return ConstComponent(*found->second);
    throw Exception("Could not find component");
}

template<class Index, class CoreComponent, class Component, class ConstComponent>
class L::Set<Index, CoreComponent, Component, ConstComponent>::iterator {
    typename std::map<Index, CoreComponent*>::iterator _it;
    public:
        explicit iterator(std::map<Index, CoreComponent*>& base, bool is_end) : _it(is_end ? base.end() : base.begin()) {}
        bool operator==(const iterator &rhs) const { return _it == rhs._it; }
        bool operator!=(const iterator &rhs) const { return _it != rhs._it; }
        Component operator++() { _it++; return Component(*_it->second); }
        Component operator*() { return Component(*_it->second); }
        Component operator->() { return Component(*_it->second); }
};

template<class Index, class CoreComponent, class Component, class ConstComponent>
class L::Set<Index, CoreComponent, Component, ConstComponent>::const_iterator {
    typename std::map<Index, CoreComponent*>::const_iterator _it;
public:
    explicit const_iterator(std::map<Index, CoreComponent*>& base, bool is_end) : _it(is_end ? base.cend() : base.cbegin()) {}
    bool operator==(const iterator &rhs) const { return _it == rhs._it; }
    bool operator!=(const iterator &rhs) const { return _it != rhs._it; }
    ConstComponent operator++() { _it++; return ConstComponent(*_it->second); }
    ConstComponent operator*() { return ConstComponent(*_it->second); }
    ConstComponent operator->() { return ConstComponent(*_it->second); }
};



#endif //ED_SOLVER_ENVIRONMENT_H
