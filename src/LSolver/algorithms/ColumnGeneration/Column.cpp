//
// Created by hlefebvr on 20/11/19.
//

#include "Column.h"

const L::Column::Coefficients &L::Column::coefficients() const {
    return _coefficients;
}

float L::Column::reduced_cost() const {
    return _reduced_cost;
}

void L::Column::reduced_cost(float r) {
    _reduced_cost = r;
}

void L::Column::coefficient(const std::string &ctr, float coef) {
    _coefficients.emplace_back(std::pair<std::string, float>( ctr, coef ));
}

void L::Column::objective_cost(float c) {
    _objective_cost = c;
}

float L::Column::objective_cost() const {
    return _objective_cost;
}

bool L::Column::empty() const {
    return _coefficients.empty();
}
