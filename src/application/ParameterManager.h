//
// Created by hlefebvr on 15/11/19.
//

#ifndef LBDS_SOLVER_PARAMETERMANAGER_H
#define LBDS_SOLVER_PARAMETERMANAGER_H

#include <string>
#include "enums.h"

namespace L {
    class ParameterManager;
}

class L::ParameterManager {
    void load_configuration(const std::string& config_file);
    void handle_key_value(const std::string& key, const std::string& value);
    friend class Application;

    // parameters
    LogLevel _log_level = Release;
    float _tolerance = 0.00000001;
    bool _external_solver_logs = false;
public:
    LogLevel log_level() const { return _log_level; }
    float tolerance() const { return _tolerance; }
    bool external_solver_logs() const { return _external_solver_logs; }
};


#endif //LBDS_SOLVER_PARAMETERMANAGER_H
