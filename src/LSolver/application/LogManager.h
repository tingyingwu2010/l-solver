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
    bool _bold;
public:
    LogManager(LogLevel level, bool bold = false) : _level(level), std::ostream(this), _bold(bold) {}
    int overflow(int obj) override {
        LogLevel param = Application::parameters().log_level();
        std::string color;
        if (_level == Release && (param == Release || param == Debug)) color = (_bold ? "\033[1;32m" : "\033[32m");
        if (_level == Debug && param == Debug) color = (_bold ? "\033[1;34m" : "\033[34m");
        if (_level == External && Application::parameters().external_solver_logs()) color = "\033[35m";
        if (!color.empty()) {
            std::cout << color;
            std::cout.put(obj);
            std::cout << "\033[0m";
        }
        return 0;
    }
};

#define _L_LOG_(level) { std::string caller = __PRETTY_FUNCTION__; L::LogManager(level, true) << caller.substr(0, caller.find("[") - 1); } L::LogManager(level) << ": "

#endif //LBDS_SOLVER_LOGMANAGER_H
