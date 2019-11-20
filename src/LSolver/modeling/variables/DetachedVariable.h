//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_DETACHEDVARIABLE_H
#define LSOLVERPROJECT_DETACHEDVARIABLE_H

#include "CoreVariable.h"

namespace L {
    class DetachedVariable;
    class Variable;
}

/**
 * \brief Implements a variable, while still being linked to a core variable.
 * \details See the Modeling Components page for more details. Like every detached object, DetachedVariable still refer
 * to a core variable, yet, can be modified independently (i.e., without modifying its core variable).
 */
class L::DetachedVariable : public CoreVariable {
    CoreVariable& _core; //!< the core constraint from which it has been detached
public:
    /**
     * \brief Constructor.
     * @param src the variable which should be detached (i.e., src's core variable is referred to by _core)
     */
    explicit DetachedVariable(const Variable& src);

    /**
     * \brief Upates its core variable's value with its own value
     */
    void update_core_value();
};


#endif //LSOLVERPROJECT_DETACHEDVARIABLE_H
