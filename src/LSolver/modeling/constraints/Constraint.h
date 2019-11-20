//
// Created by hlefebvr on 05/11/19.
//

#ifndef ED_SOLVER_CONSTRAINT_H
#define ED_SOLVER_CONSTRAINT_H

#include <string>
#include "LSolver/modeling/constraints/AbstractConstraint.h"
#include "LSolver/modeling/expressions/Expression.h"
#include "LSolver/utils/Exception.h"
#include "../variables/Variable.h"
#include "CoreConstraint.h"

namespace L {
    class Constraint;
    class ConstConstraint;
    class Environment;
}

class L::Constraint : public AbstractConstraint {
    CoreConstraint& _core;
    friend class DetachedConstraint;
public:
    // constructors
    Constraint(Environment& env, const std::string& name);
    explicit Constraint(CoreConstraint& core);
    Constraint(const Constraint& rhs) : _core(rhs._core) {}
    Constraint& operator=(const Constraint& rhs) { _core = rhs._core; return *this; }

    // getters
    [[nodiscard]] ConstraintType type() const override { return _core.type(); }
    [[nodiscard]] const Expression& expression() const override { return _core.expression(); }
    [[nodiscard]] const std::string& user_defined_name() const override { return _core.user_defined_name(); }
    [[nodiscard]] Status status() const override { return Default; }
    [[nodiscard]] float slack() const override { return _core.slack(); }
    Expression& expression() override { return _core.expression(); }
    Variable dual() override { return _core.dual(); }

    // setters
    void type(ConstraintType type) override { _core.type(type); }
    void slack(float slack) override { _core.slack(slack); }
};

class L::ConstConstraint : public AbstractConstraint {
    CoreConstraint& _core;
    void type(ConstraintType type) override {}
    //void expression(const Expression& expr) override {  }
    Expression& expression() override { throw Exception("Const constraints"); }
    void slack(float slack) override {  }
public:
    // constructor
    explicit ConstConstraint(CoreConstraint& core);
    ConstConstraint& operator=(const ConstConstraint& rhs) { _core = rhs._core; return *this; }

    // getters
    ConstraintType type() const override { return _core.type(); }
    const Expression& expression() const override { return _core.expression(); }
    const std::string& user_defined_name() const override { return _core.user_defined_name(); }
    Status status() const override { return Default; }
    Variable dual() override { return _core.dual(); }
    float slack() const override { return _core.slack(); }
};

#endif //ED_SOLVER_CONSTRAINT_H
