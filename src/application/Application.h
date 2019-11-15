//
// Created by hlefebvr on 15/11/19.
//

#ifndef LBDS_SOLVER_APPLICATION_H
#define LBDS_SOLVER_APPLICATION_H

#include <string>
#include "ParameterManager.h"
#include "enums.h"

namespace L {
    class Application;
}

class L::Application {
    static Application* _application;
    ParameterManager _parameter_manager;
    static Application& running_application();
public:
    static ParameterManager& parameters();
    static void load_configuration_file(const std::string& cfg_file);
};


#endif //LBDS_SOLVER_APPLICATION_H
