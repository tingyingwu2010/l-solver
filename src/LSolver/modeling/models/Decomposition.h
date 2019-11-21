//
// Created by hlefebvr on 21/11/19.
//

#ifndef LSOLVERPROJECT_DECOMPOSITION_H
#define LSOLVERPROJECT_DECOMPOSITION_H

#include <map>
#include <string>
#include <LSolver/application/enums.h>
#include "Model.h"

namespace L {
    class Decomposition;
}

/**
 * \brief Embeds instructions for decomposting a given model
 */
class L::Decomposition {
    Model& _src_model; //!< the model concerned by the decomposition
    std::map<std::string, VariableIndicator> _indicators; //!< the indicators specifying the way in which the source model should be decomposed
public:
    explicit Decomposition(Model& src_model);

    /**
     * \brief Adds a block indicator to the decomposition
     * @param name the name of the block associated to the indicator
     * @param indicator the actual indicator
     */
    void add_block_indicator(const std::string& name, const VariableIndicator& indicator);

    /**
     * \brief Returns the model which is concerned by the decomposition
     * @return
     */
    Model& source_model();

    /**
     * \brief Returns the list of indicators for block with their associated names
     * @return list of indicators for block with their associated names
     */
    const std::map<std::string, VariableIndicator>& block_indicators();
};


#endif //LSOLVERPROJECT_DECOMPOSITION_H
