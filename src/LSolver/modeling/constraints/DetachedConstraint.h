//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_DETACHEDCONSTRAINT_H
#define LSOLVERPROJECT_DETACHEDCONSTRAINT_H

#include "CoreConstraint.h"
#include "Constraint.h"


namespace L {
    class DetachedConstraint;
    class Constraint;
}

class L::DetachedConstraint : public CoreConstraint {
    CoreConstraint& _core;
    bool _detach_dual;
public:
    explicit DetachedConstraint(const Constraint& src, bool detach_dual = true);
    Variable dual() override;
};

#endif //LSOLVERPROJECT_DETACHEDCONSTRAINT_H
