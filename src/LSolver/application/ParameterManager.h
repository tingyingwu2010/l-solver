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
    float _tolerance = 0.00001;
    bool _external_solver_logs = false;
    float _default_artificial_cost = std::numeric_limits<float>::max();
public:
    [[nodiscard]] LogLevel log_level() const { return _log_level; }
    [[nodiscard]] float tolerance() const { return _tolerance; }
    [[nodiscard]] bool external_solver_logs() const { return _external_solver_logs; }
    [[nodiscard]] float default_artificial_cost() const { return _default_artificial_cost; }
};


#endif //LBDS_SOLVER_PARAMETERMANAGER_H
