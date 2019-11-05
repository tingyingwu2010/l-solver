//
// Created by hlefebvr on 05/11/19.
//

#ifndef ED_SOLVER_CONSTRAINT_H
#define ED_SOLVER_CONSTRAINT_H

#include <string>
#include "Expression.h"
#include "../environment/AbstractEnvironmentConstraint.h"

namespace ED {
    class Constraint;
}

class ED::Constraint {
protected:
    friend class Environment;
    friend class ConstraintVector;
    enum Type {LessOrEqualTo, GreaterOrEqualTo, EqualTo};
    Constraint* _parent = nullptr;
    Expression _expr;
    Type _type = LessOrEqualTo;
    std::string _user_defined_name;
    explicit Constraint(const std::string& user_defined_name);
public:
    Constraint(AbstractEnvironmentConstraint& env, const std::string& user_defined_name);
    void expression(const Expression& expr) { _expr = expr; }
    const Expression& expression() const { return _expr; }
    void type(Type type) { _type = type; }
    Type type() const { return _type; }

    friend std::ostream& operator<<(std::ostream& os, const Constraint& constraint);
    static std::string to_string(Type type);
};


#endif //ED_SOLVER_CONSTRAINT_H
