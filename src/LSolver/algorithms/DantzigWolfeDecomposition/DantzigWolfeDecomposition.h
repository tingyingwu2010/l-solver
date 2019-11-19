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
    unsigned long int _column_counter = 0;
    Model& _model;
    Environment _dw_env; // will contain the convex constraint and the alphas
    std::map<std::string, VariableIndicator> _indicators;
    DualAngularModel* _decomposed_model = nullptr;
    Model* _rmp_model = nullptr;
    DirectLPSolver<ExternalSolver>* _rmp_solver = nullptr;

    // related to blocks and should be refactored
    std::map<std::string, Constraint*> _convex_constraints;
    std::map<std::string, DirectLPSolver<ExternalSolver>*> _subproblem_solvers;
    std::map<std::string, Expression> _cost_functions;

    // storing core/detached variables
    std::map<std::string, DetachedConstraint*> _detached_constraints;

    Constraint& convex_constraint(const std::string& block_name);
    void actually_solve() override;
    void build_problems();
    void add_column(const Variable& variable, Model& block);
    void add_extreme_point(Model& block);
    void add_extreme_ray(Model& block);
public:
    explicit DantzigWolfeDecomposition(Model& model);
    ~DantzigWolfeDecomposition();

    // From Solver
    void save_results() override;

    // User interface to create a DualAngularModel
    void add_decomposition(const std::string& name, const VariableIndicator& indicator);
    void decompose();
};

#include "DantzigWolfeDecomposition.cpp"

#endif //LBDS_SOLVER_DANTZIGWOLFEDECOMPOSITION_H
