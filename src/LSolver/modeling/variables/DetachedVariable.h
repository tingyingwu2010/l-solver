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

class L::DetachedVariable : public CoreVariable {
    CoreVariable& _core;
public:
    explicit DetachedVariable(const Variable& src);
    void update_core_value();
};


#endif //LSOLVERPROJECT_DETACHEDVARIABLE_H
