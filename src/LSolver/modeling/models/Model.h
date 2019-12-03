//
// Created by hlefebvr on 14/11/19.
//

#ifndef ED_SOLVER_MODEL_H
#define ED_SOLVER_MODEL_H

#include <map>
#include <vector>
#include <string>
#include "LSolver/modeling/variables/Variable.h"
#include "LSolver/modeling/variables/DetachedVariable.h"
#include "LSolver/modeling/constraints/Constraint.h"
#include "LSolver/structures/MapIterator.h"
#include "LSolver/modeling/vectors/Vector.h"

namespace L {
    class Model;
    class DetachedObjective;
    class DetachedConstraint;
    class Objective;
    class VariableVector;
    std::ostream& operator<<(std::ostream& os, const Model& model);
}

/**
 * \brief Represents a model for an optimization problem.
 * \details It consists of a collection of variables, constraints and a single objective. The added components may belong to
 * different environment.
 */
class L::Model {
    std::map<std::string, Variable*> _variables; //!< variables' list
    std::map<std::string, Constraint*> _constraints; //!< constraint' list
    Objective* _objective = nullptr; //!< objective
    std::string _user_defined_name; //!< model's name
public:
    typedef MapIterator<std::string, Variable> VariableIterator;
    typedef MapIterator<std::string, Constraint> ConstraintIterator;

    /**
     * \brief Constructor.
     * @param user_defined_name model's name
     */
    explicit Model(std::string  user_defined_name = "model");
    ~Model();

    /**
     * \brief Adds a variable to the model
     * @param variable variable to be added
     */
    virtual void add(const Variable& variable);

    /**
     * \brief Adds a constraint to the model
     * @param constraint constraint to be added
     */
    virtual void add(const Constraint& constraint);

    /**
     * \brief Adds an objective to the model. Throws an exception if an objective has already been defined.
     * @param objective objective to be added
     */
    virtual void add(const Objective& objective);

    /**
     * \brief Adds every component of the vector to the model
     * @param vec vector whose coefficients are to be added
     */
    virtual void add(VariableVector& vec);

    /**
     * \brief Adds every component of the vector to the model
     * @param vec vector whose coefficients are to be added
     */
    virtual void add(ConstraintVector& vec);

    /**
     * \brief Remove the given variable. Throws an exception if it cannot be found.
     * @param variable variable to be removed
     */
    virtual void remove(const Variable& variable);

    /**
     * \brief Remove the given constraint. Throws an exception if it cannot be found.
     * @param constraint constraint to be removed
     */
    virtual void remove(const Constraint& constraint);

    /**
     * \brief Remove the given objective. Throws an exception if it cannot be found.
     * @param objective objective to be removed
     */
    virtual void remove(const Objective& objective);

    /**
     * \brief Returns the added objective. Throws an exception if no objective have been added.
     * @return objective function
     */
    Objective objective();

    /**
     * \brief Returns an iterator over the added variables
     * @return iterator over the added variables
     */
    VariableIterator variables();

    /**
     * \brief Returns an iterator over the added constraints
     * @return iterator over the added constraints
     */
    ConstraintIterator constraints();

    /**
     * \brief Returns an added constraint. Throws an exception if it cannot be found.
     * @return the added constraint
     */
    Constraint constraint(std::string name);

    /**
     * \brief Returns an added variable. Throws an exception if it cannot be found.
     * @return the added variable
     */
    Variable variable(std::string name);

    /**
     * \brief Returns the model's name
     * @return model's name
     */
    [[nodiscard]] const std::string& user_defined_name() const;

    friend std::ostream& operator<<(std::ostream& os, const Model& model);
};

#endif //ED_SOLVER_MODEL_H
