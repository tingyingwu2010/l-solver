//
// Created by hlefebvr on 25/11/19.
//


template<class Adapter_RMP, class SubProblemSolver>
L::DantzigWolfeDecompositionSolver<Adapter_RMP, SubProblemSolver>::DantzigWolfeDecompositionSolver(
        L::Decomposition &decomposition) : _decomposition(decomposition), _dw_model(decomposition) {
    _cg_iterator = new DantzigWolfeColumnIterator<SubProblemSolver>(_dw_model.sub_problems());
    _algorithm = new DantzigWolfeDecompositionAlgorithm<Adapter_RMP, SubProblemSolver>(_env, _dw_model.restricted_master_problem(), *_cg_iterator);
}

template<class Adapter_RMP, class SubProblemSolver>
L::DantzigWolfeDecompositionSolver<Adapter_RMP, SubProblemSolver>::~DantzigWolfeDecompositionSolver() {
    delete _cg_iterator;
    delete _algorithm;
}

template<class Adapter_RMP, class SubProblemSolver>
void L::DantzigWolfeDecompositionSolver<Adapter_RMP, SubProblemSolver>::actually_solve_hook() {
    _algorithm->solve();
}

template<class Adapter_RMP, class SubProblemSolver>
void L::DantzigWolfeDecompositionSolver<Adapter_RMP, SubProblemSolver>::save_results_hook() {
    Objective rmp_obj = _dw_model.restricted_master_problem().objective();
    Objective user_obj = _decomposition.source_model().objective();

    user_obj.status( rmp_obj.status() );
    user_obj.value( rmp_obj.value() );
}
