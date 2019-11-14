//
// Created by hlefebvr on 14/11/19.
//

#ifndef ED_SOLVER_DIRECTMILPSOLVER_H
#define ED_SOLVER_DIRECTMILPSOLVER_H

#include "DirectSolver.h"

namespace L {
    template<class ExternalSolver> class DirectMILPSolver;
}

template <class ExternalSolver>
class L::DirectMILPSolver : public DirectSolver {
    ExternalSolver _solver;

    void build_milp_model();
    void actually_solve() override;
    void save_results() override;
public:
    explicit DirectMILPSolver(Model& model);
};

#include "DirectMILPSolver.cpp"

#endif //ED_SOLVER_DIRECTMILPSOLVER_H
