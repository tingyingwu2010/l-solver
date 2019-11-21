//
// Created by hlefebvr on 19/11/19.
//

#ifndef LSOLVERPROJECT_DANTZIGWOLFEDECOMPOSITION_H
#define LSOLVERPROJECT_DANTZIGWOLFEDECOMPOSITION_H

#include <LSolver/modeling/models/DualAngularModel.h>
#include "../ColumnGeneration/ColumnGeneration.h"
#include "DantzigWolfeColumnIterator.h"

namespace L {
    template<class ExternalSolver> class DantzigWolfeDecomposition;
}

template<class ExternalSolver>
class L::DantzigWolfeDecomposition : public Solver {
    unsigned int _n_artificial_var = 0;
    Decomposition& _decomposition;
    DualAngularModel _model;
    Model& _rmp_as_block;
    DantzigWolfeColumnIterator<ExternalSolver>* _dw_column_generator = nullptr;
    ColumnGeneration<ExternalSolver>* _cg_solver = nullptr;

    // restricted master problem model
    Environment _dw_env;
    DetachedModel _restricted_master_problem;

    void add_artificial_variable(Constraint& ctr);
    void actually_solve() override;
    void save_results() override;
public:
    ~DantzigWolfeDecomposition();
    explicit DantzigWolfeDecomposition(Decomposition& model);

    void build_restricted_master_problem();
};

#include "DantzigWolfeDecomposition.cpp"

#endif //LSOLVERPROJECT_DANTZIGWOLFEDECOMPOSITION_H
