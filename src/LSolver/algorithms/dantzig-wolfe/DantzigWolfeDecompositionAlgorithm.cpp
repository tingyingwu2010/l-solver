//
// Created by hlefebvr on 22/11/19.
//

#include "DantzigWolfeColumnIterator.h"


template <class Adapter_RMP, class SubProblemSolver>
L::DantzigWolfeDecompositionAlgorithm<Adapter_RMP, SubProblemSolver>::DantzigWolfeDecompositionAlgorithm(Environment& env, Model& rmp, DantzigWolfeColumnIterator<SubProblemSolver>& column_generator)
    : ColumnGeneration<Adapter_RMP>(env, rmp, column_generator), _column_generator(column_generator)
{}

template<class Adapter_RMP, class SubProblemSolver>
void L::DantzigWolfeDecompositionAlgorithm<Adapter_RMP, SubProblemSolver>::save_results_hook() {
    ColumnGeneration<Adapter_RMP>::save_results_hook();
    _column_generator.reset_sub_problems_variables();
    for (auto& var_col : ColumnGeneration<Adapter_RMP>::_generated_columns) {
        for (auto& m : var_col.second.as_variables()) {
            Variable var = m.first;
            var.value( var.value() + var_col.first.value() * m.second );
        }
    }
}
