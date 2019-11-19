//
// Created by hlefebvr on 19/11/19.
//

#ifndef LSOLVERPROJECT_DANTZIGWOLFE_H
#define LSOLVERPROJECT_DANTZIGWOLFE_H

#include <LSolver/modeling/DualAngularModel.h>
#include "../ColumnGeneration/ColumnGeneration.h"

namespace L {
    template<class ExternalSolver> class DantzigWolfe;
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

template<class ExternalSolver>
class L::DantzigWolfe : public Solver {
    unsigned int _n_artificial_var = 0;
    DualAngularModel& _model;
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
    ~DantzigWolfe();
    explicit DantzigWolfe(DualAngularModel& model);

    void build_restricted_master_problem();
};

#include "DantzigWolfe.cpp"

#endif //LSOLVERPROJECT_DANTZIGWOLFE_H
