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

class L::DetachedObjective : public L::CoreObjective {
    CoreObjective& _core;
public:
    explicit DetachedObjective(const Objective& src);
    void update_core_value();
};


#endif //LSOLVERPROJECT_DETACHEDOBJECTIVE_H
