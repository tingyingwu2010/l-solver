//
// Created by hlefebvr on 03/12/19.
//

#ifndef LSOLVERPROJECT_VARIABLEVECTOR_H
#define LSOLVERPROJECT_VARIABLEVECTOR_H

#include <LSolver/modeling/vectors/Vector.h>
#include <LSolver/modeling/variables/Variable.h>

namespace L {
    class VariableVector;
}

class L::VariableVector : public Vector<Variable> {
    VariableType _type = Positive;
    unsigned int _priority = 0;
    void after_new_component_hook(Variable& var) override;
public:
    VariableVector(Environment& env, const std::string& user_defined_name, unsigned long int dimension = 1);
    void type(VariableType type);
    void priority(unsigned int priority);
};


#endif //LSOLVERPROJECT_VARIABLEVECTOR_H
