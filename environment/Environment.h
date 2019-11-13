//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_ENVIRONMENT_H
#define ED_SOLVER_ENVIRONMENT_H

#include <vector>
#include <map>
#include "../utils/Exception.h"
#include "../modeling/Constraint.h"
#include "../modeling/Variable.h"

namespace L {
    class Environment;
    template<class, class, class, class> class Set;
}

template <class Index, class CoreComponent, class Component, class ConstComponent>
class L::Set {
    std::map<Index, CoreComponent*> _components;
    Environment& _env;
public:
    class iterator;
    class const_iterator;

    explicit Set(Environment& env) : _env(env) {}
    virtual ~Set();
    Component get_or_create(const Index& index);
    ConstComponent get(const Index& index) const;
    iterator begin() { return iterator(_components, false); }
    iterator end() { return iterator(_components, true); }
    const_iterator cbegin() const { return const_iterator(_components, false); }
    const_iterator cend() const { return const_iterator(_components, true); }
};

class L::Environment  {
public:
    typedef Set<std::string, CoreVariable, Variable, ConstVariable> VariableSet;
    typedef Set<std::string, CoreConstraint, Constraint, ConstConstraint> ConstraintSet;
private:
    VariableSet _variables = VariableSet(*this);
    ConstraintSet _constraints = ConstraintSet(*this);
public:
    // Variables
    VariableSet& variables() { return _variables; }
    const VariableSet& variables() const { return _variables; }
    Variable variable(const std::string& name) { return _variables.get_or_create(name); }
    ConstVariable variable(const std::string& name) const { return _variables.get(name); }

    // Constraints
    ConstraintSet& constraints() { return _constraints; }
    const ConstraintSet& constraints() const { return _constraints; }
    Constraint constraint(const std::string& name) { return _constraints.get_or_create(name); }
    ConstConstraint constraint(const std::string& name) const { return _constraints.get(name); }
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
