//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_COLUMN_H
#define LSOLVERPROJECT_COLUMN_H

#include <vector>
#include <string>

namespace L {
    class Column;
}

class L::Column {
public:
    typedef std::vector<std::pair<Constraint, float>> CoefficientsAsConstraints;
    typedef std::vector<std::pair<Variable, float>> CoefficientsAsVariables;
private:
    CoefficientsAsConstraints _as_constraints;
    CoefficientsAsVariables _as_variables;
    float _reduced_cost = 0;
    float _objective_cost = 0;
public:
    [[nodiscard]] float reduced_cost() const;
    [[nodiscard]] const CoefficientsAsConstraints& as_constraints() const;
    [[nodiscard]] const CoefficientsAsVariables& as_variables() const;
    [[nodiscard]] float objective_cost() const;
    [[nodiscard]] bool empty() const;

    void reduced_cost(float r);
    void constraint_coefficient(const Constraint& ctr, float coef);
    void variable_coefficient(const Variable& ctr, float coef);
    void objective_cost(float c);
};

#endif //LSOLVERPROJECT_COLUMN_H
