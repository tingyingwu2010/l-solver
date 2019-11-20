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


/**
 * \brief Indirection to a core constraint
 * \details See the Modeling Objects page for implementation details and its relation with Constraint, CoreConstraint and DetachedConstraint.
 */
class L::Constraint : public AbstractConstraint {
    CoreConstraint& _core; //!< Reference to the constraint's core constraint
    friend class DetachedConstraint;
public:
    /**
     * \brief Constructor. The constructor looks for an existing core constraint in env, if none exists, one is created and
     * the object refers to the newly created object. If there already exists a core constraint with the given name, then
     * the object refers to it.
     * @param env the environment in charge of managing the memory for the core constraint's
     * @param name the constraint name
     */
    Constraint(Environment& env, const std::string& name);

    /**
     * \brief Constructor. The object is linked to its core constraints given as parameter
     * @param core the constraint's core variable
     */
    explicit Constraint(CoreConstraint& core);

    /**
     * \brief Copy constructor. The constraint now refers to the rhs's core constraint
     * @param rhs source constraint
     */
    Constraint(const Constraint& rhs) : _core(rhs._core) {}

    /**
     * Assign constructor. Same as copy constructor.
     * @param rhs source constraint
     * @return *this
     */
    Constraint& operator=(const Constraint& rhs) { _core = rhs._core; return *this; }

    // getters
    [[nodiscard]] ConstraintType type() const override { return _core.type(); }
    [[nodiscard]] const Expression& expression() const override { return _core.expression(); }
    [[nodiscard]] const std::string& user_defined_name() const override { return _core.user_defined_name(); }
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
    Variable dual() override { return _core.dual(); }
    float slack() const override { return _core.slack(); }
};

#endif //ED_SOLVER_CONSTRAINT_H
