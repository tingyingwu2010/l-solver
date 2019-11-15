//
// Created by hlefebvr on 15/11/19.
//

#ifndef LBDS_SOLVER_LOGMANAGER_H
#define LBDS_SOLVER_LOGMANAGER_H

#include <iostream>
#include "enums.h"

namespace L {
    class LogManager;
}

class L::LogManager {
    LogLevel _level;
public:
    explicit LogManager(LogLevel level) : _level(level) {}
    template <class Type> LogManager& operator<<(Type f);
};

template<typename type>
L::LogManager &L::LogManager::operator<<(type f) {
    if (_level == Release) std::cout << "\033[32m" << f << "\033[0m";
    else if (_level == Debug) std::cout << "\033[34m" << f << "\033[0m";
    return *this;
}

#endif //LBDS_SOLVER_LOGMANAGER_H
