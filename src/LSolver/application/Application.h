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

/**
 * \brief Represents an application execution. {Singleton}. Manages logs, parameters, etc.
 */
class L::Application {
    static Application* _application; //!< running application (singleton design pattern)
    ParameterManager _parameter_manager;//!< parameter manager for the application

    /**
     * \brief Returns the currently running application (or launches one)
     * @return currently running application
     */
    static Application& running_application();
public:
    /**
     * \brief Returns the parameter manager associated to the currently running application
     * @return parameter manager
     */
    static ParameterManager& parameters();

    /**
     * \brief Loads the application parameters stored in the configuration file
     * @param cfg_file configuration file filename
     */
    static void load_configuration_file(const std::string& cfg_file);
};


#endif //LBDS_SOLVER_APPLICATION_H
