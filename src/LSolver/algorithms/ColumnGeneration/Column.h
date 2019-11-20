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

#endif //LSOLVERPROJECT_COLUMN_H
