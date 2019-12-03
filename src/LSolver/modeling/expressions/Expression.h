//
// Created by hlefebvr on 13/11/19.
//

#ifndef ED_SOLVER_EXPRESSION_H
#define ED_SOLVER_EXPRESSION_H

#include <vector>
#include <string>
#include <map>
#include <functional>
#include "LSolver/utils/Exception.h"

namespace L {
    enum ExpressionType { Num, Var, Prod, Sum, Sqrt, Exp, Pow, Ln }; // cos, sin, ...
    enum ExpressionSide { Left, Right };
    enum ExpressionTraversalOrder { PostOrder, PreOrder, InOrder };
    class ConstVariable;
    class Variable;
    class CoreVariable;
    class Expression;

    // friend functions
    void transfer(ExpressionSide from_side, Expression& from_expr, ExpressionSide to_side, Expression& to_expr);
    void swap(ExpressionSide from_side, Expression& from_expr, ExpressionSide to_side, Expression& to_expr);

    // operators
    Expression operator+(const Expression& a, const Expression& b);
    Expression operator*(const Expression& a, const Expression& b);
    Expression operator/(const Expression& a, const Expression& b);
    Expression operator-(const Expression& a, const Expression& b);

    std::ostream& operator<<(std::ostream& os, ExpressionType type);
    std::ostream& operator<<(std::ostream& os, ExpressionSide type);
    std::ostream& operator<<(std::ostream& os, ExpressionTraversalOrder type);
    std::ostream& operator<<(std::ostream& os, const Expression& expr);
}

/**
 * \brief Represents a mathematical expression as an expression tree
 * \warning Most of the implemented algorithms expect an expanded form of the mathematical expressions yet no ".expand()"
 * function has been written. This is yet to be done. For now, it is, most of the time, assumed that the given expressions are
 * already expanded by the user.
 */
class L::Expression {
    static unsigned long int _expression_id; //!< general expression counter, used as id

    ExpressionType _type = Num; //!< type of the node in the expression tree
    Expression* _right = nullptr; //!< right child
    Expression* _left = nullptr; //!< left child
    float _numerical = 0; //!< if _type == Num, the numerical constant associated with the node
    CoreVariable* _variable = nullptr; //!< if _type == Var, the variable associated with the node
    unsigned long int _id = _expression_id++; //!< expression's id

    // operators
    /**
     * \brief Applies an operator ('*' or '+') on 'this' with rhs as right term
     * @param type operator ('*' or '+')
     * @param rhs right term
     * @return *this
     */
    Expression& apply_operator(ExpressionType type, const Expression& rhs);
public:
    // constructors
    /**
     * \brief Default constructor
     */
    Expression() = default;

    /**
     * \brief Creates a new expression whose root node is of the given type
     * @param type root node type
     */
    explicit Expression(ExpressionType type);

    /**
     * \brief Creates a new expression whose root is of type Var and is associated to the given variable
     * @param variable variable associated to the expression's root
     */
    Expression(const Variable& variable); // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

    /**
     * \brief Creates a new expression whose root is of type Num and has value 'coef'
     * @param coef numerical value
     */
    Expression(float coef); // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

    /**
     * \brief Copy constructor. The whole expression tree is copied.
     * @param rhs the source expression to be copied
     */
    Expression(const Expression& rhs);

    /**
     * \brief Assign constructor. The whole expression is copied
     * @param rhs the source expression to be copied
     * @return *this
     */
    Expression& operator=(const Expression& rhs);

    /**
     * \brief Destructor. Frees left and right children.
     */
    virtual ~Expression();

    // getters
    /**
     * \brief Returns the variable associated to the expression if the expression is of type Var (otherwise, throws an exception)
     * @return expression as variable
     */
    Variable as_variable();

    /**
     * \brief Returns the variable associated to the expression if the expression is of type Var (otherwise, throws an exception)
     * @return expression as variable
     */
    [[nodiscard]] ConstVariable as_variable() const;

    /**
     * \brief Returns the numerical constant associated to the expression if the expression if of type Num (otherwise, throws an exception)
     * @return expression as numerical constant
     */
    float& as_numerical();

    /**
     * \brief Returns the numerical constant associated to the expression if the expression if of type Num (otherwise, throws an exception)
     * @return expression as numerical constant
     */
    [[nodiscard]] float as_numerical() const;

    /**
     * \brief Returns true if the expression has a child on its 'side', false otherwise
     * @param side Left or Right
     * @return true if the expression has a child on its 'side', false otherwise
     */
    [[nodiscard]] bool has_child(ExpressionSide side) const;

    /**
     * \brief Returns the child node located on the expression's 'side' (Left/Right) if any, throws an exception otherwise
     * @param side Leff or Right
     * @return child node located on the expression's 'side' (Left/Right) if any, throws an exception otherwise
     */
    Expression& child(ExpressionSide side);

    /**
     * \brief Returns the child node located on the expression's 'side' (Left/Right) if any, throws an exception otherwise
     * @param side Leff or Right
     * @return child node located on the expression's 'side' (Left/Right) if any, throws an exception otherwise
     */
    [[nodiscard]] const Expression& child(ExpressionSide side) const;

    /**
     * \brief Returns expression's id
     * @return expression's id
     */
    [[nodiscard]] unsigned long int id() const;

    /**
     * \brief Returns expression's type
     * @return expression's type
     */
    [[nodiscard]] ExpressionType type() const;

    // setters
    /**
     * \brief Sets the expression's 'side' child to be 'expr'. No copy is performed. Thus, '&expr' will be destroyed by the expression's constructor.
     * @param side Left/Right
     * @param expr new 'side' child
     */
    void attach(ExpressionSide side, Expression& expr);

    /**
     * \brief Sets the expression's 'side' child to be 'expr'. The given expression is copied.
     * @param side Left/Right
     * @param expr new 'side' child to be copied
     */
    void attach_copy(ExpressionSide side, const Expression& expr);

    /**
     * \brief Sets the expression's 'side' child to be 'expr'. If the expression already has a 'side' child, it will be moved to 'expr''s side.
     * @param side Left/Right
     * @param expr child to be inserted
     */
    void insert(ExpressionSide side, Expression& expr);

    /**
     * \brief Shifts the root node to the 'side'. The expression therefore ends with a child which is the previous root.
     * @param side Left/Right
     */
    void shift(ExpressionSide side);

    /**
     * \brief Removes and deletes (frees memory) a child. For memory safety reasons, no "detach" without delete (memory free)
     * has been implemented. See transfer/swap for alternatives.
     * @param side Left/Right
     */
    void detach_delete(ExpressionSide side);

    // operations on children
    /**
     * \brief Transfer a child node from a source node to a destination node's child. No copy is performed.
     * @param from_side Left/Right
     * @param from_expr source node whose 'from_side' child will be transfered to to_expr
     * @param to_side Left/Right
     * @param to_expr destination node whose 'to_side' child will become 'from_expr''s 'from_side''s child
     */
    friend void transfer(ExpressionSide from_side, Expression& from_expr, ExpressionSide to_side, Expression& to_expr);

    /**
     * \brief Exchanges a child node of 'from_expr' with a child node of 'to_expr'.
     * @param from_side Left/Right
     * @param from_expr expression 1
     * @param to_side Left/Right
     * @param to_expr expression 2
     */
    friend void swap(ExpressionSide from_side, Expression& from_expr, ExpressionSide to_side, Expression& to_expr);

    // operators
    /**
     * \brief Addition overload. See apply_operator().
     * @param rhs right term
     * @return *this
     */
    Expression& operator+=(const Expression& rhs);

    /**
     * \brief Product overload. See apply_operator().
     * @param rhs right term
     * @return *this
     */
    Expression& operator*=(const Expression& rhs);

    /**
     * \brief Subtraction overload.
     * @param rhs right term
     * @return *this
     */
    Expression& operator-=(const Expression& rhs);

    /**
     * \brief Division overload.
     * @param rhs right term
     * @return *this
     */
    Expression& operator/=(const Expression& rhs);

    // traversal algorithms
    /**
     * \brief Generic implementation of the Depth first traversal on the expression tree. Early stop of the traversal can be performed
     * by throwing a StopIteration exception.
     * @param f lambda function to be called on each node during traversal
     * @tparam order traversal order (postfix, infix, prefix)
     */
    template<ExpressionTraversalOrder order> void depth_first_traversal(const std::function<void(Expression& expr)>& f);

    /**
     * \brief Generic implementation of the Depth first traversal on the expression tree. Early stop of the traversal can be performed
     * by throwing a StopIteration exception.
     * @param f lambda function to be called on each node during traversal
     * @tparam order traversal order (postfix, infix, prefix)
     */
    template<ExpressionTraversalOrder order> void depth_first_traversal(const std::function<void(const Expression& expr)>& f) const;

    // evaluators
    /**
     * \brief Evaluates the numerical value of an expression.
     * @param cast_variables true if the variables encountered should be casted, fase otherwise (an exception is thrown if a variable is found)
     * @return numerical result
     */
    float feval(bool cast_variables = true);

    // expression search/splits
    /**
     * \brief Given a set of variable indicators, i.e., a set of lambda function which respond true for some variables and false for others,
     * returns a set of subexpressions composed only of those variables for which the associated indicator returns true for all of its variables.
     * Note that the given indicators MUST define a separable expression in those variables. For instance, "x * y" cannot be separated by the two following
     * indicators: ```c++ [](const Variable& var){ return var.user_defined_name() == "x"; } and [](const Variable& var){ return var.user_defined_name() == "y"; } ```.
     * @param indicators map between a string (indicator's name) and a lambda function returning true for some variables, false for others.
     * @return set of subexpressions associated to each indicator by name
     */
    std::map<std::string, Expression> split_by_variable(const std::map<std::string, std::function<bool(const Variable&)>>& indicators);

    // output and display
    /**
     * \brief Returns a string expression representing the expression's root node
     * @return string expression representing the expression's root node
     */
    [[nodiscard]] std::string to_elementary_string() const;

    /**
     * \brief Returns the expression as a string
     * @return expression as a string
     */
    [[nodiscard]] std::string to_string() const;

    /**
     * \brief Writes the expression tree in a .dot file to be used with graphviz. If 'with_system_command' is set to true,
     * the linux command 'dot -Tpng' is executed.
     * @param filename the destination filename
     * @param with_system_command true if the linux command to generate a PNG file should be executed by the program.
     */
    void export_to_dot(const std::string& filename, bool with_system_command = true) const;
};

template<L::ExpressionTraversalOrder order>
void L::Expression::depth_first_traversal(const std::function<void(Expression & expr)>& explore) {

    std::function<void(Expression&)> traversal;
    traversal = [&traversal, &explore](Expression& current){
        if constexpr(order == PostOrder) {
            if (current.has_child(Left)) traversal(current.child(Left));
            if (current.has_child(Right)) traversal(current.child(Right));
            explore(current);
        } else if constexpr(order == InOrder) {
            if (current.has_child(Left)) traversal(current.child(Left));
            explore(current);
            if (current.has_child(Right)) traversal(current.child(Right));
        } else {
            explore(current);
            if (current.has_child(Left)) traversal(current.child(Left));
            if (current.has_child(Right)) traversal(current.child(Right));
        }
    };

    try { traversal(*this); } catch (const StopIteration& err) {}
}

template<L::ExpressionTraversalOrder order>
void L::Expression::depth_first_traversal(const std::function<void(const Expression & expr)>& explore) const {
    std::function<void(const Expression&)> traversal;
    traversal = [&traversal, &explore](const Expression& current){
        if constexpr(order == PostOrder) {
            if (current.has_child(Left)) traversal(current.child(Left));
            if (current.has_child(Right)) traversal(current.child(Right));
            explore(current);
        } else if constexpr(order == InOrder) {
            if (current.has_child(Left)) traversal(current.child(Left));
            explore(current);
            if (current.has_child(Right)) traversal(current.child(Right));
        } else {
            explore(current);
            if (current.has_child(Left)) traversal(current.child(Left));
            if (current.has_child(Right)) traversal(current.child(Right));
        }
    };

    try { traversal(*this); } catch (const StopIteration& err) {}
}


#endif //ED_SOLVER_EXPRESSION_H
