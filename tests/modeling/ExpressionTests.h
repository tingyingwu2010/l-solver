//
// Created by hlefebvr on 16/11/19.
//

#ifndef LSOLVERPROJECT_EXPRESSIONTESTS_H
#define LSOLVERPROJECT_EXPRESSIONTESTS_H

#include "gtest/gtest.h"
#include <LSolver/modeling/Variable.h>
#include <LSolver/modeling/Expression.h>

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

#endif //LSOLVERPROJECT_EXPRESSIONTESTS_H
