//
// Created by hlefebvr on 15/11/19.
//

#include "ParameterManager.h"
#include "../utils/Exception.h"
#include "LogManager.h"
#include <regex>
#include <fstream>
#include "enums.h"

void L::ParameterManager::load_configuration(const std::string& config_file) {

    std::ifstream f(config_file);
    if (!f.is_open()) throw Exception("Could not open configuration file");

    std::regex kv("([A-Z_]+)=([a-zA-Z0-9_.]+)(( +)#(.+))?");
    std::regex cm("(( *)#(.+))|");
    for (std::string line ; std::getline(f, line) ; ) {
        std::smatch match;
        if (std::regex_match(line, match, kv)) handle_key_value(match[1],match[2]);
        else if (std::regex_match(line, cm)) continue;
        else throw Exception("Unexpected character in line \"" + line + "\"");
    }

    f.close();
}

void L::ParameterManager::handle_key_value(const std::string& key, const std::string& value) {
    auto as_boolean = [&key, &value](){
        if (value == "TRUE") return true;
        if (value == "FALSE") return false;
        throw Exception("Parameter " + key + " takes boolean value: TRUE, FALSE.");
    };

    if (key == "LOG_LEVEL") {
        if (value == "NONE") _log_level = None;
        else if (value == "RELEASE") _log_level = Release;
        else if (value == "DEBUG") _log_level = Debug;
        else throw Exception("Unknown log level:" + value + ", allowed: RELEASE, NON, DEBUG");
    } else if (key == "TOLERANCE") {
        _tolerance = std::stof(value);
        if (_tolerance <= 0) throw Exception("Invalid tolerance value: " + value + ", allowed: positive values");
    } else if (key == "EXTERNAL_SOLVER_LOGS") {
        _external_solver_logs = as_boolean();
    } else if (key == "EXTERNAL_SOLVER_PREPROCESSING") {
        _external_solver_preprocessing = as_boolean();
    } else {
        throw Exception("Unknwon parameter " + key);
    }

    _L_LOG_(Debug) << "Parameter " << key << " was changed to " << value << "\n";
}
