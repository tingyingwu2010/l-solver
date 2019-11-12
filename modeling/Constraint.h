//
// Created by hlefebvr on 05/11/19.
//

#ifndef ED_SOLVER_CONSTRAINT_H
#define ED_SOLVER_CONSTRAINT_H

#include <string>
#include "expressions/Expression.h"
#include "../environment/AbstractEnvironmentConstraint.h"

class AbstractConstraint;
class CoreConstraint;
class Constraint;
class ConstConstraint;
class DetachedConstraint;

class AbstractConstraint {
public:
    enum Type {LessOrEqualTo, GreaterOrEqualTo, EqualTo};
    enum Status { Core, Detached, Default };

    virtual void type(Type) = 0;
    virtual Type type() const = 0;
    virtual const Expression& expression() const = 0;
    virtual void expression(const Expression& expr) = 0;
    virtual std::string user_defined_name() const = 0;
    virtual Status status() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Constraint& constraint);
    static std::string to_string(Type type);
};

class CoreConstraint : public AbstractConstraint {
protected:
    Expression _expr;
    AbstractConstraint::Type _type = LessOrEqualTo;
    std::string _user_defined_name;
public:
    explicit CoreConstraint(std::string  user_defined_name);
    void expression(const Expression& expr) override { _expr = expr; }
    const Expression& expression() const override { return _expr; }
    void type(Type type) override { _type = type; }
    Type type() const override { return _type; }
    std::string user_defined_name() const override { return _user_defined_name; }
    Status status() const override { return Core; }
};

class Constraint : public AbstractConstraint {
    CoreConstraint& _core;
    friend class DetachedConstraint;
public:
    explicit Constraint(CoreConstraint& core);
    void type(Type type) override { _core.type(type); }
    Type type() const override { return _core.type(); }
    const Expression& expression() const override { return _core.expression(); }
    void expression(const Expression& expr) override { _core.expression(expr); }
    std::string user_defined_name() const override { return _core.user_defined_name(); }
    Status status() const override { return Default; }
};

class ConstConstraint : public AbstractConstraint {
    CoreConstraint& _core;
    friend class ConstDetachedConstraint;
    void type(Type type) override {}
    void expression(const Expression& expr) override {  }
public:
    explicit ConstConstraint(CoreConstraint& core);
    Type type() const override { return _core.type(); }
    const Expression& expression() const override { return _core.expression(); }
    std::string user_defined_name() const override { return _core.user_defined_name(); }
    Status status() const override { return Default; }
};


#endif //ED_SOLVER_CONSTRAINT_H
