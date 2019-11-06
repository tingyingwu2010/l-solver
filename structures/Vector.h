//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_VECTOR_H
#define ED_SOLVER_VECTOR_H

// #include <map>
#include <vector>
#include <cstdarg>
#include <iostream>
#include "../structures/Map.h"
#include "../utils/Exception.h"

namespace ED {

    typedef std::array<unsigned int, 4> VectorIndex;

    /**
     * \brief Represents of vector of components. The components are stored as pointers.
     * \details A vector is a Map whose indices are arrays of u_int and where the access is read or create.
     * @tparam index
     * @tparam components
     */
    template<class Component, class Iterator = DefaultMapIterator<VectorIndex, Component>, class ConstIterator = DefaultConstMapIterator<VectorIndex, Component>>
    class AbstractVector : public Map<VectorIndex, Component, Iterator, ConstIterator> {
    protected:
        const unsigned int _dimension = 4;
        Component& on_component_not_found(const VectorIndex& index) override = 0;
        void add_component(const VectorIndex& index, Component& x) override { Map<VectorIndex, Component, Iterator, ConstIterator>::add_component(index, x); }
        Component& get_component(const VectorIndex& index) override { return Map<VectorIndex, Component, Iterator, ConstIterator>::get_component(index); }
        const Component& get_component(const VectorIndex& index) const override { return Map<VectorIndex, Component, Iterator, ConstIterator>::get_component(index); }
        std::array<unsigned int, 4> as_array4(unsigned int i, ...) const {
            va_list raw_indices;
            std::array<unsigned int, 4> indices = {i};
            va_start(raw_indices, i);
            for (unsigned int k = 1; k < _dimension; k += 1) indices[k] = va_arg(raw_indices, unsigned int);
            va_end(raw_indices);
            return indices;
        }
    public:

        template<class... Uint> Component& operator()(Uint... indices) { return this->get_component(as_array4(indices...)); }
        template<class... Uint> const Component& operator()(Uint... indices) const { return this->get_component(as_array4(indices...)); }

    };

}

#endif //ED_SOLVER_VECTOR_H
