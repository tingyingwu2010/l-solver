//
// Created by hlefebvr on 15/11/19.
//

#include "Application.h"
#include "../utils/Exception.h"
#include "LogManager.h"

L::Application* L::Application::_application = nullptr;

L::Application &L::Application::running_application() {
    if (!_application) _application = new Application();
    return *_application;
}

L::ParameterManager &L::Application::parameters() {
    return running_application()._parameter_manager;
}

void L::Application::load_configuration_file(const std::string &cfg_file) {
    running_application()._parameter_manager.load_configuration(cfg_file);
}
