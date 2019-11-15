//
// Created by hlefebvr on 15/11/19.
//

#ifndef LBDS_SOLVER_APPLICATION_H
#define LBDS_SOLVER_APPLICATION_H

#include <string>
#include "ParameterManager.h"
#include "enums.h"
#include "LogManager.h"

#define _L_LOG_(level) L::Application::log(level, __PRETTY_FUNCTION__)

namespace L {
    class Application;
}

class L::Application {
    static Application* _application;
    ParameterManager& _parameter_manager;
    static Application& running_application();
public:
    explicit Application(const std::string& config_file = "__use_default__");
    static ParameterManager& parameters();
    static LogManager log(LogLevel level, const std::string& caller);
};


#endif //LBDS_SOLVER_APPLICATION_H
