//
// Created by hlefebvr on 16/11/19.
//

#include "DantzigWolfeDecomposition.h"
#include "../../modeling/DualAngularModel.h"

template<class ExternalSolver>
L::DantzigWolfeDecomposition<ExternalSolver>::DantzigWolfeDecomposition(L::Model &model) : _model(model) {}

template <class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::actually_solve() {
    Solver::solve();
}

template <class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::save_results() {

}

template<class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::add_decomposition(std::string name, L::VariableIndicator indicator) {
    _indicators.insert({ name, indicator });
}

template<class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::decompose() {
    _decomposed_model = new DualAngularModel(_model, _indicators);
    _L_LOG_(Debug) << *_decomposed_model << std::endl;
}

