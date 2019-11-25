//
// Created by hlefebvr on 25/11/19.
//

#ifndef LSOLVERPROJECT_DANTZIGWOLFEDECOMPOSITIONSOLVER_H
#define LSOLVERPROJECT_DANTZIGWOLFEDECOMPOSITIONSOLVER_H

#include <LSolver/algorithms/Solver.h>
#include <LSolver/modeling/models/Decomposition.h>
#include <LSolver/environment/Environment.h>
#include "DantzigWolfeColumnIterator.h"
#include "DantzigWolfeDecompositionAlgorithm.h"

namespace L {
    template<class Adapter_RMP, class SubProblemSolver> class DantzigWolfeDecompositionSolver;
}

template<class Adapter_RMP, class SubProblemSolver>
class L::DantzigWolfeDecompositionSolver : public Solver {
    Decomposition& _decomposition;
    DantzigWolfeModel _dw_model;
    Environment _env;
    DantzigWolfeColumnIterator<SubProblemSolver>* _cg_iterator = nullptr;
    DantzigWolfeDecompositionAlgorithm<Adapter_RMP, SubProblemSolver>* _algorithm = nullptr;
    void actually_solve_hook() override;
    void save_results_hook() override;
public:
    virtual ~DantzigWolfeDecompositionSolver();
    explicit DantzigWolfeDecompositionSolver(Decomposition& decomposition);
};

#include "DantzigWolfeDecompositionSolver.cpp"

#endif //LSOLVERPROJECT_DANTZIGWOLFEDECOMPOSITIONSOLVER_H
