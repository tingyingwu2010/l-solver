//
// Created by hlefebvr on 14/11/19.
//

#ifndef ED_SOLVER_MODEL_H
#define ED_SOLVER_MODEL_H

#include <map>
#include <vector>
#include <string>
#include "Variable.h"
#include "Constraint.h"

namespace L {
    class Model;
    class Objective;
    template<class Index, class Components> class MapIterator;
}

class L::Model {
    std::map<std::string, Variable*> _variables;
    std::map<std::string, Constraint*> _constraints;
    Objective* _objective = nullptr;
    std::string _user_defined_name;
public:
    typedef MapIterator<std::string, Variable> VariableIterator;
    typedef MapIterator<std::string, Constraint> ConstraintIterator;
    explicit Model(std::string  user_defined_name = "model");
    ~Model();

    void add(const Variable& variable);
    void add(const Constraint& constraint);
    void add(const Objective& objective);

    void remove(const Variable& variable);
    void remove(const Constraint& constraint);
    void remove(const Objective& objective);

    Objective objective();
    VariableIterator variables();
    ConstraintIterator constraints();
};

template <class Index, class Component>
class L::MapIterator {
    std::map<Index, Component*>& _map;
public:
    class iterator;
    explicit MapIterator(std::map<Index, Component*>& m) : _map(m) {}
    iterator begin() { return iterator(_map, false); }
    iterator end() { return iterator(_map, true); }
};

template <class Index, class Component>
class L::MapIterator<Index, Component>::iterator {
    typename std::map<Index, Component*>::iterator _it;
public:
    iterator(std::map<Index, Component*>& m, bool is_end) : _it(!is_end ? m.begin() : m.end()) {}
    bool operator==(const iterator& rhs) const { return _it == rhs._it; }
    bool operator!=(const iterator& rhs) const { return !(*this == rhs); }
    iterator& operator++() { _it++; return *this; }
    Component operator*() const { return Component(*_it->second); }
};

#endif //ED_SOLVER_MODEL_H
