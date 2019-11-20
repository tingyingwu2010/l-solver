//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_COREOBJECTIVE_H
#define LSOLVERPROJECT_COREOBJECTIVE_H

#include "AbstractObjective.h"

namespace L {
    class CoreObjective;
}

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



#endif //LSOLVERPROJECT_COREOBJECTIVE_H
