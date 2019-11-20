//
// Created by hlefebvr on 16/11/19.
//

#ifndef LSOLVERPROJECT_EXPRESSIONTESTS_H
#define LSOLVERPROJECT_EXPRESSIONTESTS_H

#include "gtest/gtest.h"
#include <map>
#include <vector>
#include <functional>
#include <LSolver/modeling/variables/Variable.h>
#include <LSolver/modeling/expressions/Expression.h>

using namespace L;

TEST(modeling_expressions, feval_tests) {
    Environment env;
    Variable a = Variable(env, "a");
    Variable x = Variable(env, "x");
    Variable y = Variable(env, "y");
    EXPECT_EQ(Expression(10).feval(), 10);
    for (unsigned int i = 0 ; i < 20 ; i += 1) { // sort of monte carlo check
        a.value(rand() % 10);
        x.value(rand() % 10);
        y.value(rand() % 10);
        int c1 = rand() % 10;
        int c2 = rand() % 10;
        int c3 = rand() % 10;
        int c4 = rand() % 10;
        EXPECT_EQ((c1 * a + c2 * x + c3 * y + c4).feval(), c1 * a.value() + c2 * x.value() + c3 * y.value() + c4);
        EXPECT_EQ((c1 * a * c2 * x * c3 * y + c4).feval(), c1 * a.value() * c2 * x.value() * c3 * y.value() + c4);
        EXPECT_EQ((c1 * a + c2 * x * c3 * y + c4).feval(), c1 * a.value() + c2 * x.value() * c3 * y.value() + c4);
        EXPECT_EQ((c1 * a * c2 * x + c3 * y + c4).feval(), c1 * a.value() * c2 * x.value() + c3 * y.value() + c4);
        EXPECT_THROW((x + 10).feval(false), Exception);
    }
}

TEST(modeling_expressions, check_expression_simple_constructions) {
    Environment env;
    Variable a = Variable(env, "a");
    Variable x = Variable(env, "x");
    Variable y = Variable(env, "y");
    Expression expr1 = a * (x + y) + 10;
    Expression expr2 = 10 + a * x + a * y;

    for (unsigned int i = 0 ; i < 20 ; i += 1) { // sort of monte carlo check
        a.value(rand() % 10);
        x.value(rand() % 10);
        y.value(rand() % 10);
        EXPECT_EQ(expr1.feval(), expr2.feval());
    }
}

TEST(modeling_expressions, check_expression_complex_constructions) {
    Environment env;
    Variable a = Variable(env, "a");
    Variable x = Variable(env, "x");
    Variable y = Variable(env, "y");
    Variable z = Variable(env, "z");
    Expression expr1 = (2 * a + x) * (y + 2 * z) * 1 + x * x + 3 * x + 10 * a;
    Expression expr2 = 2 * a * y + 4 * a * z + x * y + 2 * x * z + x * x + 3 * x + 10 * a;

    for (unsigned int i = 0 ; i < 20 ; i += 1) { // sort of monte carlo check
        a.value(rand() % 10);
        x.value(rand() % 10);
        y.value(rand() % 10);
        EXPECT_EQ(expr1.feval(), expr2.feval());
    }
}

TEST(modeling_expressions, wrong_casting_should_throw_exception) {
    Environment env;
    Variable a = Variable(env, "a");
    Expression expr = a;
    EXPECT_THROW(expr.as_numerical(), Exception);
    expr = 10;
    EXPECT_THROW(expr.as_variable(), Exception);
    expr = a + 10;
    EXPECT_THROW(expr.as_numerical(), Exception);
    EXPECT_THROW(expr.as_variable(), Exception);
}

TEST(modeling_expressions, to_string) { // WARNING THIS TEST DEPENDS ON HOW COMMUTATIVITY IS HANDLED
    Environment env;
    Variable a = Variable(env, "a");
    Variable x = Variable(env, "x");
    Variable y = Variable(env, "y");
    EXPECT_EQ(((a + 2) * (x + y + 10)).to_string(), "(a + 2) (x + 10 + y)");
    EXPECT_EQ(((a + 2) * (x * (y + 10))).to_string(), "(a + 2) x (y + 10)");
}

TEST(modeling_expressions, split_by_variabe) {
    Environment env;
    Variable a = Variable(env, "a");
    Variable x = Variable(env, "x");
    Variable y = Variable(env, "y");
    Variable z = Variable(env, "z");
    // making a weird expression with ultiple terms on same variables
    Expression expr = 2 * x + x * x + 3 * z * z + 2 + 10 * y + a + y - 2 * a + 10;
    // here are the expected outputs:
    std::map<std::string, Expression> expected_results;
    expected_results.insert({ "a", a - 2 * a });
    expected_results.insert({ "x", 2 * x + x * x });
    expected_results.insert({ "y", 10 * y + y });
    expected_results.insert({ "_default", 12 + 3 * z * z });

    // building variable indicators
    std::map<std::string, std::function<bool(const Variable&)>> indicators;
    indicators.insert({ "x", [](const Variable& v){ return v.user_defined_name() == "x"; } });
    indicators.insert({ "y", [](const Variable& v){ return v.user_defined_name() == "y"; } });
    indicators.insert({ "a", [](const Variable& v){ return v.user_defined_name() == "a"; } });
    // computing result:
    std::map<std::string, Expression> results = expr.split_by_variable(indicators);

    // Monte Carlo check:
    for (auto expected : expected_results) {
        for (unsigned int i = 0; i < 20; i += 1) { // sort of monte carlo check
            a.value(rand() % 10);
            x.value(rand() % 10);
            y.value(rand() % 10);
            z.value(rand() % 10);
            EXPECT_EQ(expected.second.feval(), results.find(expected.first)->second.feval());
        }
    }

}

TEST(modeling_variables, bounds) {
    Environment env;
    Variable x = Variable(env, "x");
    EXPECT_EQ(x.lb(), 0);
    EXPECT_EQ(x.ub(), std::numeric_limits<float>::max());
    x.type(Free);
    EXPECT_EQ(x.lb(), std::numeric_limits<float>::lowest());
    EXPECT_EQ(x.ub(), std::numeric_limits<float>::max());
}

#endif //LSOLVERPROJECT_EXPRESSIONTESTS_H
