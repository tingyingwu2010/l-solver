//
// Created by hlefebvr on 19/11/19.
//

#ifndef LSOLVERPROJECT_COLUMNGENERATION_H
#define LSOLVERPROJECT_COLUMNGENERATION_H

#include <vector>
#include <LSolver/algorithms/DirectSolver/DirectLPSolver.h>
#include <LSolver/environment/Environment.h>
#include "LSolver/modeling/models/Model.h"
#include "../Solver.h"

namespace L {
    class Constraint;
    template<class ExternalSolver> class ColumnGeneration;
    class ColumnIterator;
    class Column;
}

/**
 * \brief Implements the generic column generation procedure.
 * \details Column generation may be applied in cases where the number of variables is very large compare to the number of
 * constraints provided that one posesses a polyhedral description of the aforementioned columns. Formally, we may consider
 * the following model
 * \f{align*}{ \textrm{minimize } & c^Tx + d^Ty \\ \textrm{s.t. } & Ax \ge b \\ & \sum_{i=j}^N t_{ij}x_j + \sum_{j|y_j\in\mathcal Y} h_{ij}y_j \ge f_i \quad \forall i = 1,...,M \f}
 * where $\mathcal Y$ is a polyhedron. 
 */
template<class ExternalSolver>
class L::ColumnGeneration : public Solver {
public:
    typedef std::vector<std::pair<Variable, Column>> VariableColumnPairs;
protected:
    Environment& _env; //!< An environment storing the added variables
    Model& _restricted_master_problem; //!< The restricted master problem model. It shall contain only a subset of columns
    DirectLPSolver<ExternalSolver>& _restricted_master_problem_solver; //!< The solver used to solve the RMP to optimality
    ColumnIterator& _column_iterator; //!< An iterator that iteratively solves the subproblem (to optimality or not) and returns columns to be addded
    VariableColumnPairs _generated_columns;  //!< Contains the set of columns generated during the algorithm

    void actually_solve() override;
    void add_column(const Column& column); //!< adds a column to the RMP
    void save_results() override;
public:
    explicit ColumnGeneration(Environment& env, Model& restricted_master_problem, ColumnIterator& column_iterator);
    ~ColumnGeneration();

    [[nodiscard]] const VariableColumnPairs& variable_column_paris() const; //!< returns a set of pairs between the column (seen as variables) and its associated variable
};

#include "ColumnGeneration.cpp"

#endif //LSOLVERPROJECT_COLUMNGENERATION_H
