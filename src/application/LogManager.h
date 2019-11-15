//
// Created by hlefebvr on 15/11/19.
//

#ifndef LBDS_SOLVER_LOGMANAGER_H
#define LBDS_SOLVER_LOGMANAGER_H

#include <iostream>
#include "Application.h"
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
    LogLevel param = Release;
    Application::parameters().log_level();
    if (_level == param) {
        std::cout << "\033[32m" << f << "\033[0m";
        return *this;
    }
    if (_level == param) {
        std::cout << "\033[34m" << f << "\033[0m";
        return *this;
    }
    return *this;
}

#define _L_LOG_(level) (L::LogManager(level) << __PRETTY_FUNCTION__)

#endif //LBDS_SOLVER_LOGMANAGER_H
