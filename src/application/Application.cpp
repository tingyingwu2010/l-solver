//
// Created by hlefebvr on 15/11/19.
//

#include "Application.h"
#include "../utils/Exception.h"
#include "LogManager.h"

L::Application* L::Application::_application = nullptr;

L::Application::Application(const std::string &config_file) : _parameter_manager(*new ParameterManager(config_file)) {
    if (_application) throw Exception("Another application is already running");
    _application = this;
}

L::Application &L::Application::running_application() {
    if (!_application) Application();
    return *_application;
}

L::ParameterManager &L::Application::parameters() {
    return running_application()._parameter_manager;
}

L::LogManager L::Application::log(L::LogLevel level, const std::string& caller) {
    return (LogManager(level) << caller << ": ");
}

