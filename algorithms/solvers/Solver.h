//
// Created by hlefebvr on 12/11/19.
//

#ifndef ED_SOLVER_SOLVER_H
#define ED_SOLVER_SOLVER_H

namespace L {
    class Solver;
}

class L::Solver {
public:
    virtual void solve() = 0;
};

#endif //ED_SOLVER_SOLVER_H
