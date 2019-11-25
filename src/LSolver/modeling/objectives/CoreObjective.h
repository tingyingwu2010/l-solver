//
// Created by hlefebvr on 20/11/19.
//

#ifndef LSOLVERPROJECT_COREOBJECTIVE_H
#define LSOLVERPROJECT_COREOBJECTIVE_H

#include "AbstractObjective.h"

namespace L {
    class CoreObjective;
}

/**
 * \brief Implements an objective.
 * \details Like every core object, core objectives are the essence of what they represent in the sense that
 * contrary to Objective's, it posesses its attributes. CoreObjective's should never be exposed to the user.
 * Only the Environment class shall be allowed for their creation. Environment is in charge of core objective's memory.
 */
class L::CoreObjective : public AbstractObjective {
protected:
    Expression _expression; //!< objective's expression (see AbstractObjective)
    std::string _user_defined_name; //!< objective's name
    ObjectiveStatus _status = Unsolved; //!< objective's status (see AbstractObjective)
    ObjectiveType _type = Minimize; //!< objective's type (see AbstractObjective)
    float _value = std::numeric_limits<float>::max(); //!< objective's value
    float _lb = std::numeric_limits<float>::lowest();
    float _ub = std::numeric_limits<float>::max();
public:
    /**
     * \brief Constructor.
     * @param user_defined_name objective's name
     */
    explicit CoreObjective(const std::string& user_defined_name);

    // getters
    [[nodiscard]] ObjectiveType type() const override;
    [[nodiscard]] const std::string& user_defined_name() const override;
    [[nodiscard]] const Expression& expression() const override;
    [[nodiscard]] ObjectiveStatus status() const override;
    [[nodiscard]] float value() const override;
    [[nodiscard]] float ub() const override;
    [[nodiscard]] float lb() const override;
    Expression& expression() override;

    // setters
    void type(ObjectiveType type) override;
    void status(ObjectiveStatus status) override;
    void value(float value) override;
    void lb(float lb) override;
    void ub(float ub) override;
};



#endif //LSOLVERPROJECT_COREOBJECTIVE_H
