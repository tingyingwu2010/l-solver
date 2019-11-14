//
// Created by hlefebvr on 14/11/19.
//

#ifndef LBDS_SOLVER_MAPITERATOR_H
#define LBDS_SOLVER_MAPITERATOR_H

#include <map>

namespace L {
    template<class Index, class Components> class MapIterator;
}


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


#endif //LBDS_SOLVER_MAPITERATOR_H
