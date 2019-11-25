//
// Created by hlefebvr on 22/11/19.
//

#ifndef LSOLVERPROJECT_DANTZIGWOLFECOLUMN_ITERATOR_H
#define LSOLVERPROJECT_DANTZIGWOLFECOLUMN_ITERATOR_H

#include "../ColumnGeneration/ColumnIterator.h"

namespace L {
    template<class SubProblemSolver> class DantzigWolfeColumnIterator;
    class DantzigWolfeSubProblem;
}

template<class SubProblemSolver>
class L::DantzigWolfeColumnIterator : public ColumnIterator {
    std::map<DantzigWolfeSubProblem*, SubProblemSolver*> _sub_problems;
    typename std::map<DantzigWolfeSubProblem*, SubProblemSolver*>::iterator _sub_problems_iterator;
    bool _last_iteration_improved = true;
    bool _is_done = false;

    void next_sub_problem();
public:
    explicit DantzigWolfeColumnIterator(const std::map<std::string, DantzigWolfeSubProblem*>&);
    DantzigWolfeColumnIterator(const DantzigWolfeColumnIterator<SubProblemSolver>& rhs);
    virtual ~DantzigWolfeColumnIterator();
    Column get_next_column() override ;
    bool is_done() override;
    void reset_sub_problems_variables();
};

#include "DantzigWolfeColumnIterator.cpp"

#endif //LSOLVERPROJECT_DANTZIGWOLFECOLUMNITERATOR_H
