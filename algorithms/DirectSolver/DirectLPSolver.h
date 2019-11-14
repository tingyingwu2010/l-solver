//
// Created by hlefebvr on 14/11/19.
//

#ifndef LBDS_SOLVER_DIRECTLPSOLVER_H
#define LBDS_SOLVER_DIRECTLPSOLVER_H

#include "DirectSolver.h"

namespace L {
    template<class ExternalSolver> class DirectLPSolver;
    class DetachedVariable;
}

template<class ExternalSolver>
class L::DirectLPSolver : public DirectSolver {
    ExternalSolver _solver;
    std::vector<DetachedVariable*> _detached_variables;

    void build_lp_model();
    void actually_solve() override;
    void save_results() override;
public:
    explicit DirectLPSolver(Model& model);
    ~DirectLPSolver();
};

#include "DirectLPSolver.cpp"

#endif //LBDS_SOLVER_DIRECTLPSOLVER_H
