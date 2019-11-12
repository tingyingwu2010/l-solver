//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_LIST_H
#define ED_SOLVER_LIST_H

#include <vector>
#include <string>

namespace L {

    template<class components>
    class List {
        std::vector<components> _components;
        typedef typename std::vector<components>::iterator iterator;
        typedef typename std::vector<components>::const_iterator const_iterator;
    public:
        // write functions
        void add(components& component);

        // read functions

        // iterators
        iterator begin() { return _components.begin(); }
        iterator end() { return _components.end(); }
        const_iterator cbegin() const { return _components.cbegin(); }
        const_iterator cend() const { return _components.cend(); }
    };

    template<class components>
    void List<components>::add(components &component) {
        _components.push_back(component);
    }

    class Variable;
    class Constraint;

    typedef List<bool> BoolList;
    typedef List<int> IntList;
    typedef List<float> FloatList;
    typedef List<std::string> StringList;
    typedef List<Variable*> VariablePtrList;
    typedef List<Variable* const> VariableConstPtrList;
    typedef List<Constraint*> ConstraintPtrList;
    typedef List<Constraint* const> ConstraintConstPtrList;

};


#endif //ED_SOLVER_LIST_H
