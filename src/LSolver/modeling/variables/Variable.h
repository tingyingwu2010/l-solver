//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_VARIABLE_H
#define ED_SOLVER_VARIABLE_H

#include <string>
#include "../enums.h"
#include "AbstractVariable.h"
#include "CoreVariable.h"

namespace L {
    class Variable;
    class ConstVariable;
    class DetachedVariable;
    class Environment;
}

/**
 * \brief Indirection to a core variable
 * \details See the Modeling Objects page for implementation details and its relation with Variable, CoreVariable and DetachedVariable.
 */
class L::Variable : public AbstractVariable {
protected:
    CoreVariable& _core; //!< Reference to the constraint's core constraint
    friend class Expression;
    friend class DetachedVariable;
public:
    /**
     * \brief Constructor. The constructor looks for an existing core variable in env, if none exists, one is created and
     * the object refers to the newly created object. If there already exists a core variable with the given name, then
     * the object refers to it.
     * @param env the environment in charge of managing the memory for the core variable's
     * @param name the variable name
     */
    explicit Variable(Environment& env, const std::string& name);

    /**
     * \brief Constructor. The object is linked to its core variable given as parameter
     * @param core the variable's core variable
     */
    explicit Variable(CoreVariable& core);

    [[nodiscard]] float value() const override { return _core.value(); }
    [[nodiscard]] float ub() const override { return _core.ub(); }
    [[nodiscard]] float lb() const override { return _core.lb(); }
    [[nodiscard]] float reduced_cost() const override { return _core.reduced_cost(); }
    [[nodiscard]] const std::string& user_defined_name() const override { return _core.user_defined_name(); }
    [[nodiscard]] VariableType type() const override { return _core.type(); }
    [[nodiscard]] unsigned int priority() const override { return _core.priority(); }

    // setters
    void value(float value) override { _core.value(value); }
    void ub(float ub) override { _core.ub(ub); }
    void lb(float lb) override { _core.lb(lb); }
    void reduced_cost(float reduced_cost) override { _core.reduced_cost(reduced_cost); }
    void type(VariableType type) override { _core.type(type); }
    void priority(unsigned int priority) override { _core.priority(priority); }
    void force_value_to(float value) override { _core.force_value_to(value); }
};


class L::ConstVariable : public AbstractVariable {
    const CoreVariable& _core;
    void value(float value) override {  }
    void ub(float ub) override {  }
    void lb(float lb) override {  }
    void reduced_cost(float reduced_cost) override {  }
    void type(VariableType type) override {  }
    void priority(unsigned int priority) override {  }
    void force_value_to(float value) override {  }
public:
    explicit ConstVariable(const CoreVariable& core);
    float value() const override { return _core.value(); }
    float ub() const override { return _core.ub(); }
    float lb() const override { return _core.lb(); }
    float reduced_cost() const override { return _core.reduced_cost(); }
    [[nodiscard]] const std::string& user_defined_name() const override { return _core.user_defined_name(); }
    VariableType type() const override { return _core.type(); }
    unsigned int priority() const override { return _core.priority(); }
};



#endif //ED_SOLVER_VARIABLE_H
