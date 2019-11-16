//
// Created by hlefebvr on 16/11/19.
//

#ifndef LBDS_SOLVER_DANTZIGWOLFEDECOMPOSITION_H
#define LBDS_SOLVER_DANTZIGWOLFEDECOMPOSITION_H

#include "../Solver.h"

namespace L {
    template<class ExternalSolver> class DantzigWolfeDecomposition;
}

template<class ExternalSolver>
class L::DantzigWolfeDecomposition : public Solver {
    void actually_solve() override;
public:
    explicit DantzigWolfeDecomposition(Model& model);
    void save_results() override;
};

#include "DantzigWolfeDecomposition.cpp"

#endif //LBDS_SOLVER_DANTZIGWOLFEDECOMPOSITION_H
