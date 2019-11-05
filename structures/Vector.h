//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_VECTOR_H
#define ED_SOLVER_VECTOR_H

// #include <map>
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
    template<class component>
    class Vector : public Map<VectorIndex, component> {
    protected:
        component& on_component_not_found(const VectorIndex& index) override = 0;
        void add_component(const VectorIndex& index, component& x) override { Map<VectorIndex, component>::add_component(index, x); }
    public:

        // \todo these should be written with va_list...
        component& operator()(unsigned int i) { return this->get_component({i}); }
        component& operator()(unsigned int i, unsigned int j) { return this->get_component({i,j}); }
        component& operator()(unsigned int i, unsigned int j, unsigned int k) { return this->get_component({i,j,k}); }
        component& operator()(unsigned int i, unsigned int j, unsigned int k, unsigned int l) { return this->get_component({i,j,k,l}); }

        const component& operator()(unsigned int i) const { return this->get_component({i}); }
        const component& operator()(unsigned int i, unsigned int j) const { return this->get_component({i,j}); }
        const component& operator()(unsigned int i, unsigned int j, unsigned int k) const { return this->get_component({i,j,k}); }
        const component& operator()(unsigned int i, unsigned int j, unsigned int k, unsigned int l) const { return this->get_component({i,j,k,l}); }
    };

    class Variable;
    class Constraint;
    typedef Vector<Variable> AbstractVariableVector;
    typedef Vector<Constraint> AbstractConstraintVector;

}

#endif //ED_SOLVER_VECTOR_H
