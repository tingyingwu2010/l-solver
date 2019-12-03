//
// Created by hlefebvr on 12/11/19.
//

#ifndef ED_SOLVER_SOLVER_H
#define ED_SOLVER_SOLVER_H

#include <gmpxx.h>

namespace L {
    class Solver;
}

/**
 * \brief Implements the interface for solvers
 */
class L::Solver {
protected:
    float _last_execution_time = std::numeric_limits<float>::max(); //!< stores the execution time of the last execution

    /**
     * \brief Solves the optimization problem
     */
    virtual void actually_solve_hook() = 0;

    /**
     * \brief Saves the results
     */
    virtual void save_results_hook() = 0;

    /**
     * \brief Hook for performing some actions before save_results_hook() is called
     */
    virtual void before_saving_results_hook() {}

    /**
     * \brief Hook for performing some actions before actually_solve_hook() is called
     */
    virtual void before_actually_solving_hook() {}
public:
    /**
     * \brief Destructor.
     */
    virtual ~Solver() = default;

    /**
     * \brief Calls actually_solve_hook() and measure its execution time.
     */
    virtual void solve();

    /**
     * \brief Returns the last measured execution time
     * @return last measured execution time
     */
    float last_execution_time();
};

#endif //ED_SOLVER_SOLVER_H
