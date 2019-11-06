//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_MAP_H
#define ED_SOLVER_MAP_H

#include <map>
#include "../utils/Exception.h"

namespace ED {

    template<class Index, class Component> class DefaultMapIterator;
    template<class Index, class Component> class DefaultConstMapIterator;

    /**
     * Implements a map linking indices and pointers to components
     * @tparam Index
     * @tparam Component
     * @tparam Iterator
     * @tparam ConstIterator
     */
    template<class Index, class Component, class Iterator = DefaultMapIterator<Index, Component>, class ConstIterator = DefaultConstMapIterator<Index, Component>>
    class Map {
    protected:
        std::map<Index, Component *> _components;

        virtual Component& on_component_not_found(const Index& index);
    public:
        typedef Iterator iterator;
        typedef ConstIterator const_iterator;

        virtual iterator begin() { return iterator(_components, false); }
        virtual iterator end() { return iterator(_components, true); }

        virtual const_iterator cbegin() { return const_iterator(_components, false); }
        virtual const_iterator cend() { return const_iterator(_components, true); }

        virtual Component& get_component(const Index& index);
        virtual const Component& get_component(const Index& index) const;
        virtual void add_component(const Index& index, Component& x);
    };

    template<class index, class Component>
    class DefaultMapIterator {
        typename std::map<index, Component *>::iterator _it;
    public:
        explicit DefaultMapIterator(std::map<index, Component*> &components, bool is_end) {
            _it = is_end ? components.end() : components.begin();
        }
        bool operator==(const DefaultMapIterator &rhs) const { return _it == rhs._it; }
        bool operator!=(const DefaultMapIterator &rhs) const { return _it != rhs._it; }
        Component &operator++() { _it++; return *_it->second; }
        Component &operator*() { return *_it->second; }
        Component *operator->() { return _it->second; }
    };

    template<class index, class component>
    class DefaultConstMapIterator {
        typename std::map<index, component *>::const_iterator _it;
    public:
        explicit DefaultConstMapIterator(std::map<index, component*> &components, bool is_end) {
            _it = is_end ? components.cend() : components.cbegin();
        }
        bool operator==(const DefaultConstMapIterator &rhs) const { return _it == rhs._it; }
        bool operator!=(const DefaultConstMapIterator &rhs) const { return _it != rhs._it; }
        const component &operator++() { _it++; return *_it->second; }
        const component &operator*() { return *_it->second; }
        const component *operator->() { return _it->second; }
    };

    template<class Index, class component, class Iterator, class ConstIterator>
    component &Map<Index, component, Iterator, ConstIterator>::get_component(const Index &index) {
        auto found = _components.find(index);
        return (found == _components.end()) ? on_component_not_found(index) : *found->second;
    }

    template<class Index, class component, class Iterator, class ConstIterator>
    const component &Map<Index, component, Iterator, ConstIterator>::get_component(const Index &index) const {
        auto found = _components.find(index);
        if (found == _components.end()) throw Exception("Could not find requested Component");
        return *found->second;
    }

    template<class Index, class component, class Iterator, class ConstIterator>
    void Map<Index, component, Iterator, ConstIterator>::add_component(const Index &index, component &x) {
        _components.insert({ index, &x });
    }

    template<class Index, class component, class Iterator, class ConstIterator>
    component &Map<Index, component, Iterator, ConstIterator>::on_component_not_found(const Index &index) {
        throw Exception("Could not find requested Component");
    }

}

#endif //ED_SOLVER_MAP_H
