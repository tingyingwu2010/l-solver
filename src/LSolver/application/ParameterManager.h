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

/**
 * \brief Manages the parameters at the application level.
 * Every parameter can be modified through a configuration file where each parameter's name correspond to
 * an attribute of this class writen in UPPER_CASE, e.g., LOG_LEVEL for _log_level. All parameters are optional.
 */
class L::ParameterManager {
    /**
     * \brief Loads all parameters from the given configuration file.
     * @param config_file configuration file filename
     */
    void load_configuration(const std::string& config_file);

    /**
     * \brief Updates the parameters based on a (key, value) match as read by the load_configuration() method.
     * @param key parameter name
     * @param value value as string
     */
    void handle_key_value(const std::string& key, const std::string& value);
    friend class Application;

    // parameters
    LogLevel _log_level = Release; //!< Log level of the running application
    float _tolerance = 0.00001; //!< tolerance for float comparison
    bool _external_solver_logs = false; //!< true if the logs from the external solvers shall be printed out, false otherwise
    bool _external_solver_preprocessing = false; //!< true if the preprocessing from the external solver shall be turned on, false otherwise
public:
    /**
     * \brief Returns the LOG_LEVEL parameter
     * @return LOG_LEVEL
     */
    [[nodiscard]] LogLevel log_level() const { return _log_level; }

    /**
     * \brief Returns the TOLERANCE parameter
     * @return TOLERANCE
     */
    [[nodiscard]] float tolerance() const { return _tolerance; }

    /**
     * \brief Returns the EXTERNAL_SOLVER_LOGS parameter
     * @return EXTERNAL_SOLVER_LOGS
     */
    [[nodiscard]] bool external_solver_logs() const { return _external_solver_logs; }

    /**
     * \bref Returns the EXTERNAL_SOLVER_PREPROCESSING parameter
     * @return EXTERNAL_SOLVER_PREPROCESSING
     */
    [[nodiscard]] bool external_solver_preprocessing() const { return _external_solver_preprocessing; }
};


#endif //LBDS_SOLVER_PARAMETERMANAGER_H
