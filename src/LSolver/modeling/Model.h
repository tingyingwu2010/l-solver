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
    class DetachedModel;
    class DetachedObjective;
    class Objective;
    std::ostream& operator<<(std::ostream& os, const Model& model);
}

class L::Model {
    std::map<std::string, Variable*> _variables;
    std::map<std::string, Constraint*> _constraints;
    Objective* _objective = nullptr;
    std::string _user_defined_name;
    std::map<unsigned int, unsigned int> _user_arguments;
public:
    typedef MapIterator<std::string, Variable> VariableIterator;
    typedef MapIterator<std::string, Constraint> ConstraintIterator;
    explicit Model(std::string  user_defined_name = "model");
    ~Model();

    virtual void add(const Variable& variable);
    virtual void add(const Constraint& constraint);
    virtual void add(const Objective& objective);

    virtual void remove(const Variable& variable);
    virtual void remove(const Constraint& constraint);
    virtual void remove(const Objective& objective);

    Objective objective();
    VariableIterator variables();
    ConstraintIterator constraints();
    Constraint constraint(const std::string name);

    const std::string& user_defined_name() const;
    unsigned int user_argument(unsigned int index) const;
    void user_argument(unsigned int index, unsigned int value);

    friend std::ostream& operator<<(std::ostream& os, const Model& model);
};

class L::DetachedModel : public L::Model {
    Model& _src;
    std::vector<DetachedVariable*> _detached_variables;
    std::vector<DetachedConstraint*> _detached_constraints;
    DetachedObjective* _detached_objective = nullptr;
public:
    explicit DetachedModel(Model& src);
    ~DetachedModel();
    void add_detached(const Variable& variable);
    void add_detached(const Constraint& ctr);
    void add_detached(const Objective& obj);

    void update_primal_values();
    void update_objective_value();
};

#endif //ED_SOLVER_MODEL_H
