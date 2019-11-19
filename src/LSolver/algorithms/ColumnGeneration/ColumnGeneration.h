//
// Created by hlefebvr on 19/11/19.
//

#ifndef LSOLVERPROJECT_COLUMNGENERATION_H
#define LSOLVERPROJECT_COLUMNGENERATION_H

#include <vector>
#include <LSolver/algorithms/DirectSolver/DirectLPSolver.h>
#include <LSolver/environment/Environment.h>
#include "../../modeling/Model.h"
#include "../Solver.h"

namespace L {
    class Constraint;
    template<class ExternalSolver> class ColumnGeneration;
    class ColumnIterator;
    class Column;
}

class L::Column {
public:
    typedef std::vector<std::pair<std::string, float>> Coefficients;
private:
    Coefficients _coefficients;
    float _reduced_cost = 0;
    float _objective_cost = 0;
public:
    float reduced_cost() const;
    const Coefficients& coefficients() const;
    float objective_cost() const;

    void reduced_cost(float r);
    void coefficient(const std::string& ctr, float coef);
    void objective_cost(float c);
};

class L::ColumnIterator {
public:
    virtual Column get_next_column() = 0;
    virtual bool is_done() = 0;
};

template<class ExternalSolver>
class L::ColumnGeneration : public Solver {
    Environment _cg_env;
    Model& _restricted_master_problem;
    DirectLPSolver<ExternalSolver>& _restricted_master_problem_solver;
    ColumnIterator& _column_iterator;
    std::vector<Column> _generated_columns;

    void actually_solve() override;
    void add_column(const Column& column);
    void save_results() override { /* TODO */ }
public:
    explicit ColumnGeneration(Model& model, ColumnIterator& column_iterator);
    ~ColumnGeneration();
};

#include "ColumnGeneration.cpp"

#endif //LSOLVERPROJECT_COLUMNGENERATION_H
