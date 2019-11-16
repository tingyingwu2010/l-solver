//
// Created by hlefebvr on 16/11/19.
//

template<class ExternalSolver>
L::DantzigWolfeDecomposition<ExternalSolver>::DantzigWolfeDecomposition(L::Model &model) {}

template <class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::actually_solve() {
    Solver::solve();
}

template <class ExternalSolver>
void L::DantzigWolfeDecomposition<ExternalSolver>::save_results() {

}

