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

/**
 * \brief Implements a constraint, while still being linked to a core constraint.
 * \details See the Modeling Components page for more details. Like every detached object, DetachedConstraints still refer
 * to a core constraint, yet, can be modified independently (i.e., without modifying its core constraint).
 */
class L::DetachedConstraint : public CoreConstraint {
    CoreConstraint& _core; //!< the core constraint from which it has been detached
    bool _detach_dual; //!< this parameter is used to indicate whether or not the associated dual variable shall be detached or not. (i.e., if the dual() function shall return the core's dual or a new dual variable)
public:
    /**
     * \brief Constructor.
     * @param src the constraint which should be detached (i.e., src's core constraint is referred to by _core)
     * @param detach_dual true if the dual variable of the constraint should also be detached, false otherwise.
     */
    explicit DetachedConstraint(const Constraint& src, bool detach_dual = true);

    Variable dual() override;
};

#endif //LSOLVERPROJECT_DETACHEDCONSTRAINT_H
