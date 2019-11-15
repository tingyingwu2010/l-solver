//
// Created by hlefebvr on 15/11/19.
//

#include "ParameterManager.h"
#include "../utils/Exception.h"
#include "Application.h"
#include <utility>
#include <regex>
#include <fstream>
#include <iostream>
#include "enums.h"

L::ParameterManager::ParameterManager(std::string config_file) : _config_file(std::move(config_file)) {
    load_configuration();
}

void L::ParameterManager::load_configuration() {
    if (_config_file == "__default__") return;

    std::ifstream f(_config_file);
    if (!f.is_open()) throw Exception("Could not open configuration file");

    std::regex kv("([A-Z_]+)=([a-zA-Z0-9_.]+)(( +)#(.+))?");
    for (std::string line ; std::getline(f, line) ; ) {
        std::smatch match;
        if (std::regex_match(line, match, kv))
            handle_key_value(match[1],match[2]);
    }

    f.close();
}

void L::ParameterManager::handle_key_value(const std::string& key, const std::string& value) {
    if (key == "LOG_LEVEL") {
        if (value == "NONE") _log_level = None;
        else if (value == "RELEASE") _log_level = Release;
        else if (value == "DEBUG") _log_level = Debug;
        else throw Exception("Unknown log level:" + value + ", allowed: RELEASE, NON, DEBUG");
    } else if (key == "TOLERANCE") {
        _tolerance = std::stof(value);
        if (_tolerance <= 0) throw Exception("Invalid tolerance value: " + value + ", allowed: positive values");
    }

    _L_LOG_(Debug) << "Parameter " << key << " was changed to " << value << "\n";
}
