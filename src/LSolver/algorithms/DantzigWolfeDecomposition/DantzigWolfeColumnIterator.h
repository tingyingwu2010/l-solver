//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_DANTZIGWOLFECOLUMNITERATOR_H
#define LSOLVERPROJECT_DANTZIGWOLFECOLUMNITERATOR_H

#include "../ColumnGeneration/ColumnIterator.h"
#include "LSolver/modeling/models/Model.h"
#include "LSolver/modeling/models/DualAngularModel.h"
#include "../DirectSolver/DirectLPSolver.h"

namespace L {
    template<class ExternalSolver> class DantzigWolfeColumnIterator;
}

template<class ExternalSolver>
class L::DantzigWolfeColumnIterator : public ColumnIterator {
    Model& _restricted_master_problem;
    DualAngularModel& _dual_angular_model;
    typedef std::pair< DetachedModel*, DirectLPSolver<ExternalSolver>*> ModelSolverAssoc;
    bool _last_iteration_improved = false;
    bool _is_done = false;
    std::vector<ModelSolverAssoc> _subproblems;
    typename std::vector<ModelSolverAssoc>::iterator _subproblem_iterator;

    void next_subproblem();
    void build_subproblems();
public:
    explicit DantzigWolfeColumnIterator(Model& rmp, DualAngularModel& da_model);
    ~DantzigWolfeColumnIterator();
    Column get_next_column() override;
    bool is_done() override;
};

#include "DantzigWolfeColumnIterator.cpp"

#endif //LSOLVERPROJECT_DANTZIGWOLFECOLUMNITERATOR_H
