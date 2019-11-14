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
    float _last_execution_time = std::numeric_limits<float>::max();
    virtual void actually_solve() = 0;
    virtual void save_results() = 0;
public:
    virtual void solve();
    float last_execution_time();
};

#endif //ED_SOLVER_SOLVER_H
