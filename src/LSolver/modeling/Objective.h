//
// Created by hlefebvr on 14/11/19.
//

#ifndef ED_SOLVER_OBJECTIVE_H
#define ED_SOLVER_OBJECTIVE_H

#include "Expression.h"
#include "Row.h"

namespace L {
    class Environment;
    class AbstractObjective;
    class CoreObjective;
    class ConstObjective;
    class DetachedObjective;
    class Objective;
    enum ObjectiveType { Minimize, Maximize };
    enum ObjectiveStatus { Unsolved, Optimal, Feasible, Infeasible, Unbounded, Error };
    std::ostream& operator<<(std::ostream& os, ObjectiveStatus obj);
    std::ostream& operator<<(std::ostream& os, ObjectiveType type);
    std::ostream& operator<<(std::ostream& os, const AbstractObjective& obj);
}

class L::AbstractObjective : public Row {
public:
    // getters
    virtual ObjectiveType type() const =  0;
    virtual const std::string& user_defined_name() const = 0;
    virtual ObjectiveStatus status() const = 0;
    virtual float value() const = 0;

    // setters
    virtual void type(ObjectiveType) = 0;
    virtual void status(ObjectiveStatus) = 0;
    virtual void value(float) = 0;

    friend std::ostream& operator<<(std::ostream& os, const AbstractObjective& obj);
};

class L::CoreObjective : public AbstractObjective {
protected:
    Expression _expression;
    std::string _user_defined_name;
    ObjectiveStatus _status = Unsolved;
    ObjectiveType _type = Minimize;
    float _value = std::numeric_limits<float>::max();
public:
    // constructor
    explicit CoreObjective(const std::string& user_defined_name);

    // getters
    ObjectiveType type() const override;
    const std::string& user_defined_name() const override;
    const Expression& expression() const override;
    Expression& expression() override;
    ObjectiveStatus status() const override;
    float value() const override;

    // setters
    void type(ObjectiveType type) override;
    void status(ObjectiveStatus status) override;
    void value(float value) override;
};

class L::Objective : public AbstractObjective {
    CoreObjective& _core;
    friend class DetachedObjective;
public:
    // constructor
    Objective(CoreObjective&);
    Objective(const Objective&);
    explicit Objective(Environment& env, const std::string& user_defined_name);

    // getters
    ObjectiveType type() const override;
    const std::string& user_defined_name() const override;
    const Expression& expression() const override;
    Expression& expression() override;
    ObjectiveStatus status() const override;
    float value() const override;

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

class L::DetachedObjective : public L::CoreObjective {
    CoreObjective& _core;
public:
    explicit DetachedObjective(const Objective& src);
    void update_core_value();
};

#endif //ED_SOLVER_OBJECTIVE_H
