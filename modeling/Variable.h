//
// Created by hlefebvr on 04/11/19.
//

#ifndef ED_SOLVER_VARIABLE_H
#define ED_SOLVER_VARIABLE_H

#include <string>
#include "../environment/AbstractEnvironmentVariable.h"

namespace ED {

    class AbstractVariable {
    public:
        enum Status { Core, Detached, Default };
        enum Type { Positive, Negative, Free, Binary, Integer };

        virtual ~AbstractVariable() = default;

        // getters
        virtual float value() const = 0; //!< access variable's value
        virtual float ub() const = 0; //!< access variable's upper bound
        virtual float lb() const = 0; //!< access variable's lower bound
        virtual float reduced_cost() const = 0;//!< access variable's reduced cost (when appropriate)
        virtual std::string user_defined_name() const = 0;//!< access variable's name
        virtual Type type() const = 0; //!< access variable's type
        virtual Status status() const = 0;//!< access variables's status

        // setters
        virtual void value(float) = 0; //!< updates variable's value
        virtual void ub(float) = 0; //!< updates variable's upper bound
        virtual void lb(float) = 0; //!< updates variable's lower bound
        virtual void reduced_cost(float) = 0; //!< updates variable's reduced cost
        virtual void type(Type) = 0; //!< updates variable's type

        friend std::ostream& operator<<(std::ostream& os, const AbstractVariable& var);
    };

    /***
     * \brief A core variable is an object which represents a decisional variable. Core variable belong to environments.
     */
    class CoreVariable : public AbstractVariable {
    protected:
        float _value = 0.0; //!< current value
        float _ub = 0.0; //!< lower bound
        float _lb = 0.0; //!< upper bound
        float _reduced_cost = 0.0; //!< reduced cost
        Type _type = Positive; //!< variable's type
        const std::string _user_defined_name;
    public:
        explicit CoreVariable(std::string user_defined_name);
        float value() const override { return _value; }
        float ub() const override { return _ub; }
        float lb() const override { return _lb; }
        float reduced_cost() const override { return _reduced_cost; }
        std::string user_defined_name() const override { return _user_defined_name; }
        Type type() const override { return _type; }
        Status status() const { return Core; }

        // setters
        void value(float value) override { _value = value; }
        void ub(float ub) override { _ub = ub; }
        void lb(float lb) override { _lb = lb; }
        void reduced_cost(float reduced_cost) override { _reduced_cost = reduced_cost; }
        void type(Type type) override { _type = type; }
    };

    /***
     * \brief A variable is a representation of a core variable. Every modification to a variable is repeated to its core variable.
     * They may be seen as indirections to core variables.
     */
    class Variable : public AbstractVariable {
    protected:
        CoreVariable& _core;
        friend class DetachedVariable;
    public:
        explicit Variable(CoreVariable& core);
        float value() const override { return _core.value(); }
        float ub() const override { return _core.ub(); }
        float lb() const override { return _core.lb(); }
        float reduced_cost() const override { return _core.reduced_cost(); }
        std::string user_defined_name() const override { return _core.user_defined_name(); }
        Type type() const override { return _core.type(); }
        Status status() const { return Default; }

        // setters
        void value(float value) override { _core.value(value); }
        void ub(float ub) override { _core.ub(ub); }
        void lb(float lb) override { _core.lb(lb); }
        void reduced_cost(float reduced_cost) override { _core.reduced_cost(reduced_cost); }
        void type(Type type) override { _core.type(type); }
    };

    class ConstVariable : public AbstractVariable {
        const CoreVariable& _core;
        void value(float value) override {  }
        void ub(float ub) override {  }
        void lb(float lb) override {  }
        void reduced_cost(float reduced_cost) override {  }
        void type(Type type) override {  }
    public:
        explicit ConstVariable(const CoreVariable& core);
        float value() const override { return _core.value(); }
        float ub() const override { return _core.ub(); }
        float lb() const override { return _core.lb(); }
        float reduced_cost() const override { return _core.reduced_cost(); }
        std::string user_defined_name() const override { return _core.user_defined_name(); }
        Type type() const override { return _core.type(); }
        Status status() const { return Default; }
    };

    /***
     * \brief Detached variables are variables which are independent of their core variables but by value.
     * Modifications which are repeated to the core variable are: value, reduced cost.
     * Modifications which are independently performed: upper bound, lower bound, type.
     */
    class DetachedVariable : public Variable {
    protected:
        float _ub = 0.0; //!< lower bound
        float _lb = 0.0; //!< upper bound
        Type _type = Positive; //!< variable's type
    public:
        explicit DetachedVariable(Variable& variable);
        float ub() const override { return _ub; }
        float lb() const override { return _lb; }
        Type type() const override { return _type; }
        Status status() const { return Detached; }

        // setters
        void ub(float ub) override { _ub = ub; }
        void lb(float lb) override { _lb = lb; }
        void type(Type type) override { _type = type; }
    };

};

#endif //ED_SOLVER_VARIABLE_H
