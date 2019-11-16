//
// Created by hlefebvr on 12/11/19.
//

#ifndef ED_SOLVER_DIRECTSOLVER_H
#define ED_SOLVER_DIRECTSOLVER_H

#include "../Solver.h"

namespace L {
    class DirectSolver;
    class Model;
}

/**
 * Solves an optimization model using a given solver adapter
 */
class L::DirectSolver : public Solver {
protected:
    Model& _model;
public:
    explicit DirectSolver(Model& model);
};


#endif //ED_SOLVER_DIRECTSOLVER_H
