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
#include "../structures/MapIterator.h"

namespace L {
    class Model;
    class Objective;
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

#endif //ED_SOLVER_MODEL_H
