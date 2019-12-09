//
// Created by hlefebvr on 09/12/19.
//

#include "SubPlot.h"

void L::SubPlot::title(const std::string &title) {
    _title = title;
}

void L::SubPlot::xlabel(const std::string &xlabel) {
    _xlabel = xlabel;
}

void L::SubPlot::ylabel(const std::string &ylabel) {
    _ylabel = ylabel;
}

const std::string &L::SubPlot::xlabel() const {
    return _xlabel;
}

const std::string &L::SubPlot::title() const {
    return _title;
}

const std::string &L::SubPlot::ylabel() const {
    return _ylabel;
}

