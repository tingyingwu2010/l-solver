//
// Created by hlefebvr on 12/11/19.
//

#ifndef ED_SOLVER_SOLVERADAPTER_H
#define ED_SOLVER_SOLVERADAPTER_H

#include "LSolver/modeling/constraints/Constraint.h"
#include "LSolver/modeling/variables/Variable.h"
#include "LSolver/modeling/objectives/Objective.h"

namespace L {
    class SolverAdapter;
}

/**
 * \brief Implements the interface for solver adapters
 */
class L::SolverAdapter {
public:
    /**
     * \brief Destructor. Should clear all the allocated components.
     */
    virtual ~SolverAdapter() = default;

    // creating models
    /**
     * \brief Creates a new variable for the external solver
     * @param variable the variable to be created
     */
    virtual void create_variable(const Variable& variable) = 0;

    /**
     * \brief Creates a new constraint for the external solver
     * @param variable the constraint to be created
     */
    virtual void create_constraint(const Constraint& constraint) = 0;

    /**
     * \brief Creates a new objective for the external solver
     * @param variable the objective to be created
     */
    virtual void create_objective(const Objective& objective) = 0;

    // solving models
    /**
     * \brief Solves the build model. (does not save the results back in L-Solver, see save_resuts())
     */
    virtual void solve() = 0;

    /**
     * \brief Saves the results from the last solve() call to the L-Solver
     * @param primal true if the primal values should be solved, false otherwise
     * @param dual true if the dual values should be solved, false otherwise (may throw an exception if dual values are required to be saved
     * and no dual information has been found (e.g., MILPs)
     */
    virtual void save_results(bool primal = true, bool dual = false) = 0;

    // output and displays
    /**
     * \brief Exports a textual representation of the model to a file
     * @param filename destination filename
     */
    virtual void export_to_file(const std::string& filename) = 0;
};

#endif //ED_SOLVER_SOLVERADAPTER_H
