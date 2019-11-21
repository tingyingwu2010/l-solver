//
// Created by hlefebvr on 17/11/19.
//

#ifndef LSOLVERPROJECT_DUALANGULARMODEL_H
#define LSOLVERPROJECT_DUALANGULARMODEL_H

#include "LSolver/application/enums.h"
#include "LSolver/modeling/models/Model.h"
#include "LSolver/environment/Environment.h"
#include "Decomposition.h"

namespace L {
    class DualAngularModel;
    class LinkingConstraint;

    std::ostream& operator<<(std::ostream& os, const DualAngularModel& model);
}

class L::LinkingConstraint : public Constraint {
    std::map<std::string, Expression> _splitted_expression;
public:
    LinkingConstraint(const Constraint& ctr, std::map<std::string, Expression> block_splitted_expression);
    Expression& block(const std::string& block_name);
};

/**
 * \brief Represents a model which has a dual angular structure
 * \details Dual angular models are characterized by a set of "blocks" and a set of linking constraints which link the blocks
 * between one another. This structure is used in several decomposition methods such as the Dantzig-Wolfe decomposition or
 * Bender's decomposition. Note that the DualAngularModel offers a "view" on a given model and is not a Model per se. It is
 * completely depentent on its source model from which it was extracted. It may loose its significance as the source model
 * gets modified. For instance, if the original model changes structure and renders the computed decomposition deprecated,
 * the DualAngularModel has to re-decompose the original model fully.
 */
class L::DualAngularModel {
    Decomposition _decomposition;
    Environment _dw_env;//!< an environment for storing the objective functions related to each blocks
    std::map<std::string, Model*> _blocks; //!< the list of blocks, represented as models
    std::map<std::string, LinkingConstraint*> _linking_constraints; //!< the list of linking constraints

    /**
     * \brief Adds the given variable its corresponding block
     * @param row variable to be dispatched
     */
    void dispatch_in_blocks(Variable &row);

    /**
     * \brief Splits the given constraints and dispatches it in the corresponding blocks. This function also creates a linking constraint (if the given constraint, in fact, does link several blocks).
     */
    void dispatch_in_blocks(Constraint &constraint);

    /**
     * \brief Splits the given objective and dispatches it in the corresponding blocks
     */
    void dispatch_in_blocks(const Objective &objective);
public:
    typedef MapIterator<std::string, Model, Model&> BlockIterator;
    typedef MapIterator<std::string, LinkingConstraint, LinkingConstraint&> LinkingConstraintIterator;

    /**
     * \brief Constructor. If this constructor is used, indicators for decomposition shall be provided though add_block_indicator() and the decompose() function
     * shall be called to effectively decompose the given model.
     * @param model the source model to be decomposed
     */
    explicit DualAngularModel(Decomposition& decomposition);

    /**
     * \brief Returns a given block
     * @param name the name of the block
     * @return the block's model
     */
    Model& block(const std::string& name);

    /**
     * \brief Returns an iterator over the defined blocks
     * @return iterator over the blocks
     */
    BlockIterator blocks();

    /**
     * \brief Returns an iterator over the linking constraints
     * @return iterator over linking constraints
     */
    LinkingConstraintIterator linking_constraints();

    /**
     * \brief Decomposes the source model with the help of the provided _indicators
     */
    void decompose();

    friend std::ostream& operator<<(std::ostream& os, const DualAngularModel& model);
};


#endif //LSOLVERPROJECT_DUALANGULARMODEL_H
