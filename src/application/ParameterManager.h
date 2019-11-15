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
    const std::string _config_file;
    void load_configuration();
    void handle_key_value(const std::string& key, const std::string& value);

    // parameters
    LogLevel _log_level = None;
    float _tolerance = 0.00000001;
public:
    explicit ParameterManager(std::string  config_file);
};


#endif //LBDS_SOLVER_PARAMETERMANAGER_H
