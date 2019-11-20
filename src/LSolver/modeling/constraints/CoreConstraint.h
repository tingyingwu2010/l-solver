//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_CORECONSTRAINT_H
#define LSOLVERPROJECT_CORECONSTRAINT_H

#include "../expressions/Expression.h"
#include "AbstractConstraint.h"

namespace L {
    class CoreConstraint;
}

class L::CoreConstraint : public AbstractConstraint {
protected:
    Expression _expr;
    ConstraintType _type = GreaterOrEqualTo;
    std::string _user_defined_name;
    CoreVariable* _dual_variable = nullptr;
    float _slack = 0;
public:
    // constructors
    explicit CoreConstraint(std::string user_defined_name);

    // getters
    Expression& expression() override { return _expr; }
    [[nodiscard]] const Expression& expression() const override { return _expr; }
    [[nodiscard]] ConstraintType type() const override { return _type; }
    [[nodiscard]] const std::string& user_defined_name() const override { return _user_defined_name; }
    [[nodiscard]] Status status() const override { return Core; }
    Variable dual() override;
    [[nodiscard]] float slack() const override { return _slack; }

    // setters
    void type(ConstraintType type) override { _type = type; }
    void slack(float slack) override { _slack = slack; }
};


#endif //LSOLVERPROJECT_CORECONSTRAINT_H
