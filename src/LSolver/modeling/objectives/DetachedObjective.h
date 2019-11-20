//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_DETACHEDOBJECTIVE_H
#define LSOLVERPROJECT_DETACHEDOBJECTIVE_H

#include "CoreObjective.h"
#include "Objective.h"

namespace L {
    class DetachedObjective;
}

/**
 * \brief Implements an objective, while still being linked to a core objective.
 * \details See the Modeling Components page for more details. Like every detached object, DetachedObjective still refer
 * to a core objective, yet, can be modified independently (i.e., without modifying its core objective).
 */
class L::DetachedObjective : public L::CoreObjective {
    CoreObjective& _core;
public:
    /**
     * \brief Constructor.
     * @param src the objective which should be detached (i.e., src's core objective is referred to by _core)
     */
    explicit DetachedObjective(const Objective& src);

    /**
     * \brief Updates its core objective's value with its own value
     */
    void update_core_value();
};


#endif //LSOLVERPROJECT_DETACHEDOBJECTIVE_H
