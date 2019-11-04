//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_MAP_H
#define ED_SOLVER_MAP_H

#include <map>
#include "../utils/Exception.h"

namespace ED {

/**
 * Implements a map linking indices and pointers to components
 * @tparam Index
 * @tparam component
 */
    template<class Index, class component>
    class Map {
    protected:
        std::map<Index, component *> _components;

        class iterator;
        class const_iterator;

        component& get_component(const Index& index);
        component& get_component(const Index& index) const;
        virtual component& on_component_not_found(const Index& index);
    public:
        iterator begin() { return iterator(_components, false); }
        iterator end() { return iterator(_components, true); }

        const_iterator cbegin() { return const_iterator(_components, false); }
        const_iterator cend() { return const_iterator(_components, true); }

        virtual void add_component(const Index& index, component& x);
    };

    template<class index, class component>
    class Map<index, component>::iterator {
        typename std::map<index, component *>::iterator _it;
    public:
        explicit iterator(std::map<index, component> &components, bool is_end) {
            _it = is_end ? components.end() : components.begin();
        }

        bool operator==(const iterator &rhs) const { return _it == rhs._it; }

        bool operator!=(const iterator &rhs) const { return _it != rhs._it; }

        component &operator++() {
            _it++;
            return *_it->second;
        }

        component &operator*() { return *_it->second; }

        component *operator->() { return _it->second; }
    };

    template<class index, class component>
    class Map<index, component>::const_iterator {
        typename std::map<index, component *>::const_iterator _it;
    public:
        explicit const_iterator(std::map<index, component> &components, bool is_end) {
            _it = is_end ? components.cend() : components.cbegin();
        }

        bool operator==(const iterator &rhs) const { return _it == rhs._it; }

        bool operator!=(const iterator &rhs) const { return _it != rhs._it; }

        const component &operator++() {
            _it++;
            return *_it->second;
        }

        const component &operator*() { return *_it->second; }

        const component *operator->() { return _it->second; }
    };

    template<class Index, class component>
    component &Map<Index, component>::get_component(const Index &index) {
        auto found = _components.find(index);
        return (found == _components.end()) ? on_component_not_found(index) : *found->second;
    }

    template<class Index, class component>
    component &Map<Index, component>::get_component(const Index &index) const {
        auto found = _components.find(index);
        if (found == _components.end()) throw Exception("Could not find requested component");
        return *found->second;
    }

    template<class Index, class component>
    void Map<Index, component>::add_component(const Index &index, component &x) {
        _components.insert({ index, &x });
    }

    template<class Index, class component>
    component &Map<Index, component>::on_component_not_found(const Index &index) {
        throw Exception("Could not find requested component");
    }

    class VariableVector;

    typedef Map<std::string, VariableVector> MapVariableVector;

}

#endif //ED_SOLVER_MAP_H
