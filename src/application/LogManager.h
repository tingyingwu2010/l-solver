//
// Created by hlefebvr on 15/11/19.
//

#ifndef LBDS_SOLVER_LOGMANAGER_H
#define LBDS_SOLVER_LOGMANAGER_H

#include <iostream>
#include <sstream>
#include "Application.h"
#include "enums.h"

namespace L {
    class LogManager;
}

class L::LogManager : public std::ostream, std::streambuf {
    LogLevel _level;
public:
    explicit LogManager(LogLevel level) : _level(level), std::ostream(this) {}
    int overflow(int obj) override {
        LogLevel param = Application::parameters().log_level();
        if (_level == Release && (param == Release || param == Debug)) {
            std::cout << "\033[32m";
            std::cout.put(obj);
            std::cout << "\033[0m";
        }
        if (_level == Debug && param == Debug) {
            std::cout << "\033[34m";
            std::cout.put(obj);
            std::cout << "\033[0m";
        }
        if (_level == External && Application::parameters().external_solver_logs()) {
            std::cout << "\033[35m";
            std::cout.put(obj);
            std::cout << "\033[0m";
        }
        return 0;
    }
};

#define _L_LOG_(level) (L::LogManager(level) << __PRETTY_FUNCTION__ << ": ")

#endif //LBDS_SOLVER_LOGMANAGER_H
