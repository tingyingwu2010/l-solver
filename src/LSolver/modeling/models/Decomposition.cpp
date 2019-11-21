//
// Created by hlefebvr on 21/11/19.
//

#include "Decomposition.h"

L::Decomposition::Decomposition(L::Model &src_model) : _src_model(src_model) {}

void L::Decomposition::add_block_indicator(const std::string &name, const L::VariableIndicator &indicator) {
    _indicators.insert({ name, indicator });
}

L::Model &L::Decomposition::source_model() {
    return _src_model;
}

const std::map<std::string, L::VariableIndicator> &L::Decomposition::block_indicators() {
    return _indicators;
}
