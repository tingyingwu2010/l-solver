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

/**
 * \brief Implements a constraint.
 * \details Like every core object, core constraints are the essence of what they represent in the sense that
 * contrary to Constraint's, it posesses its attributes. CoreConstraint's should never be exposed to the user.
 * Only the Environment class shall be allowed for their creation. Environment is in charge of core constraint's memory.
 */
class L::CoreConstraint : public AbstractConstraint {
protected:
    Expression _expr; //!< the constraint's expresion (see AbstractConstraint)
    ConstraintType _type = GreaterOrEqualTo; //!< the constraint's type (see AbstractConstraint)
    std::string _user_defined_name; //!< the constraint's name
    CoreVariable* _dual_variable = nullptr; //!< the dual variable associated to the constraint (note that it is NULL by default, see dual() )
    float _slack = 0; //!< constraint's slack value
public:
    /**
     * \brief Constructor.
     * @param user_defined_name the name of the new constraint
     */
    explicit CoreConstraint(std::string user_defined_name);

    // getters
    Expression& expression() override { return _expr; }
    [[nodiscard]] const Expression& expression() const override { return _expr; }
    [[nodiscard]] ConstraintType type() const override { return _type; }
    [[nodiscard]] const std::string& user_defined_name() const override { return _user_defined_name; }
    Variable dual() override;
    [[nodiscard]] float slack() const override { return _slack; }

    // setters
    void type(ConstraintType type) override { _type = type; }
    void slack(float slack) override { _slack = slack; }
};


#endif //LSOLVERPROJECT_CORECONSTRAINT_H
