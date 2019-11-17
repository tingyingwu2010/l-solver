//
// Created by hlefebvr on 15/11/19.
//

#ifndef LBDS_SOLVER_ENUMS_H
#define LBDS_SOLVER_ENUMS_H

#include <functional>

namespace L {
    class Variable;
    enum LogLevel { None, Release, Debug, External };
    typedef std::function<bool(const Variable&)> VariableIndicator;
}

#endif //LBDS_SOLVER_ENUMS_H
