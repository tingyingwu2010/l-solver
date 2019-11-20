//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_DETACHEDMODEL_H
#define LSOLVERPROJECT_DETACHEDMODEL_H

#include "Model.h"

namespace L {
    class DetachedModel;
}

/**
 * \brief Detached version of Model.
 * \details Regular Variable, Constraint and Objective may be added as well. The detached model is responsible for its
 * detached compoents' memory.
 */
class L::DetachedModel : public L::Model {
    Model& _src; //!< model from which it has been detached
    std::vector<DetachedVariable*> _detached_variables; //!<
    std::vector<DetachedConstraint*> _detached_constraints;
    DetachedObjective* _detached_objective = nullptr;
public:
    /**
     * \brief Constructor. Copies every component of src in a detached way.
     * @param src source model
     */
    explicit DetachedModel(Model& src);
    ~DetachedModel();

    /**
     * \brief Creates a new detached variable based on its argument.
     * @param variable the variable to be added
     */
    void add_detached(const Variable& variable);

    /**
     * \brief Creates a new detached constraint based on its argument.
     * @param variable the constraint to be added
     */
    void add_detached(const Constraint& ctr);

    /**
     * \brief Creates a new detached objective based on its argument.
     * @param variable the objective to be added
     */
    void add_detached(const Objective& obj);

    void add(const Variable& variable) override { Model::add(variable); }
    void add(const Constraint& ctr) override { Model::add(ctr); }
    void add(const Objective& obj) override { Model::add(obj); }

    /**
     * \brief Adds a detached variable to the model. Not that the DetachedModel is now responsible for the variable's destruction.
     * If this behaviour is not wanted. It is advised to call add(Variable(variable)) instead of add(variable).
     * @param variable the variable to be added
     */
    void add(DetachedVariable& variable);

    /**
     * \brief Adds a detached constraint to the model. Not that the DetachedModel is now responsible for the constraint's destruction.
     * If this behaviour is not wanted. It is advised to call add(Constraint(ctr)) instead of add(ctr).
     * @param variable the constraint to be added
     */
    void add(DetachedConstraint& ctr);

    /**
     * \brief Adds a detached objective to the model. Not that the DetachedModel is now responsible for the objective's destruction.
     * If this behaviour is not wanted. It is advised to call add(Objective(obj)) instead of add(obj).
     * @param variable the variable to be added
     */
    void add(DetachedObjective& obj);

    // \todo should write objective() because it is ambiguous
    // \todo should write constraints() because it is ambiguous
    // \todo should write variables() because it is ambiguous
    // \todo as well for constraint(str) and variable(str)

    /**
     * \brief Updates each of its variables' core with their own value
     */
    void update_primal_values();

    /**
     * \brief Updates the objective's core value with its own value (in the case where the objective is detached. Otherwise, has no effect)
     */
    void update_objective_value();
};



#endif //LSOLVERPROJECT_DETACHEDMODEL_H
