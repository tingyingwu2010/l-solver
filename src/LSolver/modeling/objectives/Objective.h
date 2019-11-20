//
// Created by hlefebvr on 14/11/19.
//

#ifndef ED_SOLVER_OBJECTIVE_H
#define ED_SOLVER_OBJECTIVE_H

#include "LSolver/modeling/expressions/Expression.h"
#include "LSolver/modeling/objectives/AbstractObjective.h"
#include "../enums.h"

namespace L {
    class Environment;
    class AbstractObjective;
    class CoreObjective;
    class ConstObjective;
    class DetachedObjective;
    class Objective;
}

/**
 * \brief Indirection to a core objective
 * \details See the Modeling Objects page for implementation details and its relation with Objective, CoreObjective and DetachedObjective.
 */
class L::Objective : public AbstractObjective {
    CoreObjective& _core; //!< Reference to the constraint's core objective
    friend class DetachedObjective;
public:
    /**
     * \brief Constructor. The constructor looks for an existing core objective in env, if none exists, one is created and
     * the object refers to the newly created object. If there already exists a core objective with the given name, then
     * the object refers to it.
     * @param env the environment in charge of managing the memory for the core objective's
     * @param name the objective name
     */
    Objective(Environment& env, const std::string& user_defined_name);

    /**
     * \brief Constructor. The object is linked to its core objective given as parameter
     * @param core the objective's core variable
     */
    explicit Objective(CoreObjective&);

    /**
     * \brief Copy constructor. The objective now refers to the rhs's core objective
     * @param rhs source objective
     */
    Objective(const Objective&);

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

#endif //ED_SOLVER_OBJECTIVE_H
