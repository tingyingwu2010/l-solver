//
// Created by hlefebvr on 12/11/19.
//

#ifndef ED_SOLVER_SOLVERADAPTER_H
#define ED_SOLVER_SOLVERADAPTER_H

#include "../modeling/Constraint.h"
#include "../modeling/Variable.h"
#include "../modeling/Objective.h"

namespace L {
    class SolverAdapter;
}

/**
 * \brief Implements the interface for solver adapters
 */
class L::SolverAdapter {
public:
    // creating models
    virtual void create_variable(const Variable& variable) = 0;
    virtual void create_constraint(const Constraint& constraint) = 0;
    virtual void create_objective(const Objective& objective) = 0;

    // solving models
    virtual void solve() = 0;
    virtual void save_results(bool primal = true, bool dual = false) = 0;

    // output and displays
    virtual void export_to_file(const std::string& filename) = 0;
};

#endif //ED_SOLVER_SOLVERADAPTER_H
