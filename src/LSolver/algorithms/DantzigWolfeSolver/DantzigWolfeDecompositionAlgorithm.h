//
// Created by hlefebvr on 22/11/19.
//

#ifndef LSOLVERPROJECT_DANTZIGWOLFEDECOMPOSITIONALGORITHM_H
#define LSOLVERPROJECT_DANTZIGWOLFEDECOMPOSITIONALGORITHM_H

#include "DantzigWolfeModel.h"
#include "DantzigWolfeColumnIterator.h"
#include "../ColumnGeneration/ColumnGeneration.h"

namespace L {
    template<class Adapter_RMP, class SubProblemSolver> class DantzigWolfeDecompositionAlgorithm;
    template<class SubProblemSolver> class DantzigWolfeColumnIterator;
}

template <class Adapter_RMP, class SubProblemSolver>
class L::DantzigWolfeDecompositionAlgorithm : public ColumnGeneration<Adapter_RMP> {
    DantzigWolfeColumnIterator<SubProblemSolver>& _column_generator;
    void save_results_hook() override;
public:
    explicit DantzigWolfeDecompositionAlgorithm(Environment& env, Model& rmp, DantzigWolfeColumnIterator<SubProblemSolver>& column_generator);
};

#include "DantzigWolfeDecompositionAlgorithm.cpp"

#endif //LSOLVERPROJECT_DANTZIGWOLFEDECOMPOSITIONALGORITHM_H
