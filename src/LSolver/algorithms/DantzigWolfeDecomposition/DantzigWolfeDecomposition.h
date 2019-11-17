//
// Created by hlefebvr on 16/11/19.
//

#ifndef LBDS_SOLVER_DANTZIGWOLFEDECOMPOSITION_H
#define LBDS_SOLVER_DANTZIGWOLFEDECOMPOSITION_H

#include "../Solver.h"

namespace L {
    class DualAngularModel;
    template<class ExternalSolver> class DantzigWolfeDecomposition;
}

template<class ExternalSolver>
class L::DantzigWolfeDecomposition : public Solver {
    void actually_solve() override;
    Model& _model;
    std::map<std::string, VariableIndicator> _indicators;
    DualAngularModel* _decomposed_model = nullptr;
public:
    explicit DantzigWolfeDecomposition(Model& model);
    void save_results() override;
    void add_decomposition(std::string name, VariableIndicator indicator);
    void decompose();
};

#include "DantzigWolfeDecomposition.cpp"

#endif //LBDS_SOLVER_DANTZIGWOLFEDECOMPOSITION_H
