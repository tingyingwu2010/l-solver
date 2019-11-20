//
// Created by hlefebvr on 14/11/19.
//

#ifndef ED_SOLVER_OBJECTIVE_H
#define ED_SOLVER_OBJECTIVE_H

#include "LSolver/modeling/expressions/Expression.h"
#include "LSolver/modeling/objectives/AbstractObjective.h"
#include "../enums.h"

namespace L {
    class Environment;
    class AbstractObjective;
    class CoreObjective;
    class ConstObjective;
    class DetachedObjective;
    class Objective;
}

class L::Objective : public AbstractObjective {
    CoreObjective& _core;
    friend class DetachedObjective;
public:
    // constructor
    explicit Objective(CoreObjective&);
    Objective(const Objective&);
    Objective(Environment& env, const std::string& user_defined_name);

    // getters
    [[nodiscard]] ObjectiveType type() const override;
    [[nodiscard]] const std::string& user_defined_name() const override;
    [[nodiscard]] const Expression& expression() const override;
    [[nodiscard]] ObjectiveStatus status() const override;
    [[nodiscard]] float value() const override;
    Expression& expression() override;

    // setters
    void type(ObjectiveType type) override;
    void status(ObjectiveStatus status) override;
    void value(float value) override;
};

class L::ConstObjective : public AbstractObjective {
    CoreObjective& _core;
    void type(ObjectiveType type) override;
    void status(ObjectiveStatus status) override;
    void value(float value) override;
    Expression& expression() override;
public:
    // constructor
    ConstObjective(CoreObjective&);

    // getters
    ObjectiveType type() const override;
    const std::string& user_defined_name() const override;
    const Expression& expression() const override;
    ObjectiveStatus status() const override;
    float value() const override;
};

#endif //ED_SOLVER_OBJECTIVE_H
