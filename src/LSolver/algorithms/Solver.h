//
// Created by hlefebvr on 12/11/19.
//

#ifndef ED_SOLVER_SOLVER_H
#define ED_SOLVER_SOLVER_H

#include <gmpxx.h>

namespace L {
    class Solver;
}

class L::Solver {
protected:
    float _last_execution_time = std::numeric_limits<float>::max();
    virtual void actually_solve_hook() = 0;
    virtual void save_results_hook() = 0;
    virtual void before_saving_results_hook() {}
    virtual void before_actually_solving_hook() {}
public:
    virtual ~Solver() = default;
    virtual void solve();
    float last_execution_time();
};

#endif //ED_SOLVER_SOLVER_H
