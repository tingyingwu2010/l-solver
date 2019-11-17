//
// Created by hlefebvr on 05/11/19.
//

#ifndef ED_SOLVER_CONSTRAINT_H
#define ED_SOLVER_CONSTRAINT_H

#include <string>
#include "Expression.h"
#include "../utils/Exception.h"
#include "Variable.h"

namespace L {
    class AbstractConstraint;
    class CoreConstraint;
    class Constraint;
    class ConstConstraint;
    class Environment;
    std::ostream& operator<<(std::ostream& os, const AbstractConstraint& constraint);
}

class L::AbstractConstraint {
public:
    enum Type {LessOrEqualTo, GreaterOrEqualTo, EqualTo};
    enum Status { Core, Default };

    // getters
    virtual Type type() const = 0;
    virtual const Expression& expression() const = 0;
    virtual Expression& expression() = 0;
    virtual std::string user_defined_name() const = 0;
    virtual Status status() const = 0;
    virtual Variable dual() = 0;
    virtual float slack() const = 0;

    // setters
    virtual void expression(const Expression& expr) = 0;
    virtual void type(Type) = 0;
    virtual void slack(float s) = 0;

    static std::string to_string(Type type);
};

class L::CoreConstraint : public AbstractConstraint {
protected:
    Expression _expr;
    AbstractConstraint::Type _type = LessOrEqualTo;
    std::string _user_defined_name;
    CoreVariable* _dual_variable = nullptr;
    float _slack = 0;
public:
    // constructors
    explicit CoreConstraint(std::string user_defined_name);

    // getters
    Expression& expression() override { return _expr; }
    const Expression& expression() const override { return _expr; }
    Type type() const override { return _type; }
    std::string user_defined_name() const override { return _user_defined_name; }
    Status status() const override { return Core; }
    Variable dual() override;
    float slack() const override { return _slack; }

    // setters
    void expression(const Expression& expr) override { _expr = expr; }
    void type(Type type) override { _type = type; }
    void slack(float slack) override { _slack = slack; }
};

class L::Constraint : public AbstractConstraint {
    CoreConstraint& _core;
public:
    // constructors
    Constraint(Environment& env, const std::string& name);
    explicit Constraint(CoreConstraint& core);
    Constraint(const Constraint& rhs) : _core(rhs._core) {}
    Constraint& operator=(const Constraint& rhs) { _core = rhs._core; return *this; }

    // getters
    Type type() const override { return _core.type(); }
    Expression& expression() override { return _core.expression(); }
    const Expression& expression() const override { return _core.expression(); }
    std::string user_defined_name() const override { return _core.user_defined_name(); }
    Status status() const override { return Default; }
    Variable dual() override { return _core.dual(); }
    float slack() const override { return _core.slack(); }

    // setters
    void type(Type type) override { _core.type(type); }
    void expression(const Expression& expr) override { _core.expression(expr); }
    void slack(float slack) override { _core.slack(slack); }
};

class L::ConstConstraint : public AbstractConstraint {
    CoreConstraint& _core;
    void type(Type type) override {}
    void expression(const Expression& expr) override {  }
    Expression& expression() override { throw Exception("Const constraint"); }
    void slack(float slack) override {  }
public:
    // constructor
    explicit ConstConstraint(CoreConstraint& core);
    ConstConstraint& operator=(const ConstConstraint& rhs) { _core = rhs._core; return *this; }

    // getters
    Type type() const override { return _core.type(); }
    const Expression& expression() const override { return _core.expression(); }
    std::string user_defined_name() const override { return _core.user_defined_name(); }
    Status status() const override { return Default; }
    Variable dual() override { return _core.dual(); }
    float slack() const override { return _core.slack(); }
};

#endif //ED_SOLVER_CONSTRAINT_H
