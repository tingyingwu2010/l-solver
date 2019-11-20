//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_COREVARIABLE_H
#define LSOLVERPROJECT_COREVARIABLE_H

#include "AbstractVariable.h"

namespace L {
    class CoreVariable;
}

/**
 * \brief Implements a variable.
 * \details Like every core object, core variables are the essence of what they represent in the sense that
 * contrary to Variable's, it posesses its attributes. CoreVariable's should never be exposed to the user.
 * Only the Environment class shall be allowed for their creation. Environment is in charge of core variable's memory.
 */
class L::CoreVariable : public AbstractVariable {
protected:
    float _value = 0.0; //!< current value
    float _ub = std::numeric_limits<float>::max(); //!< lower bound
    float _lb = 0.0; //!< upper bound
    float _reduced_cost = 0.0; //!< reduced cost
    VariableType _type = Positive; //!< variable's type
    const std::string _user_defined_name; //!< variable's name
    unsigned int _priority = 0; //!< variable's priotity for branching
public:
    /**
     * \brief Constructor.
     * @param user_defined_name the name of the new variable
     */
    explicit CoreVariable(std::string user_defined_name);
    [[nodiscard]] float value() const override { return _value; }
    [[nodiscard]] float ub() const override { return _ub; }
    [[nodiscard]] float lb() const override { return _lb; }
    [[nodiscard]] float reduced_cost() const override { return _reduced_cost; }
    [[nodiscard]] const std::string& user_defined_name() const override { return _user_defined_name; }
    [[nodiscard]] VariableType type() const override { return _type; }
    [[nodiscard]] unsigned int priority() const override { return _priority; }

    // setters
    void value(float value) override;
    void ub(float ub) override;
    void lb(float lb) override;
    void reduced_cost(float reduced_cost) override { _reduced_cost = reduced_cost; }
    void type(VariableType type) override;
    void priority(unsigned int priority) override { _priority = priority; }
    void force_value_to(float value) override;
};


#endif //LSOLVERPROJECT_COREVARIABLE_H
