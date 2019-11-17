//
// Created by hlefebvr on 16/11/19.
//

#include "gtest/gtest.h"
#include <LSolver/modeling/Variable.h>
#include <LSolver/environment/Environment.h>

using namespace L;

TEST(modeling_variables, checking_default_values_for_newly_created_variables) {
    const std::string name = "x";
    Environment env;
    Variable x(env, name);
    EXPECT_EQ(x.type(), Positive);
    EXPECT_EQ(x.lb(), 0);
    EXPECT_EQ(x.ub(), std::numeric_limits<float>::max());
    EXPECT_EQ(x.user_defined_name(), name);
    EXPECT_EQ(x.reduced_cost(), 0);
}

TEST(modeling_variables, a_copied_variable_should_still_refer_to_the_same_core_variable) {
    Environment env;
    Variable x = Variable(env, "x");
    Variable copy_x = x;
    x.value(100);
    x.reduced_cost(100);
    copy_x.type(Binary);
    EXPECT_EQ(copy_x.value(), x.value());
    EXPECT_EQ(copy_x.reduced_cost(), x.reduced_cost());
    EXPECT_EQ(copy_x.type(), x.type());
    EXPECT_EQ(copy_x.lb(), x.lb());
    EXPECT_EQ(copy_x.ub(), x.ub());
    EXPECT_EQ(copy_x.user_defined_name(), x.user_defined_name());
}

TEST(modeling_variables, a_detached_variable_should_update_its_core_variable_only_when_requested) {
    Environment env;
    Variable x = Variable(env, "x");
    DetachedVariable detached = DetachedVariable(x);
    x.value(100);
    EXPECT_EQ(x.value(), 100); // checking that x's value is well 100, to validate the following tests
    EXPECT_EQ(detached.value(), 0); // changing x's value should not have impacted the detached variable
    detached.update_core_value();
    EXPECT_EQ(detached.value(), 0); // updating core variable should not have changed the detacged variable's value
    EXPECT_EQ(x.value(), 0); // but should have impacted its core variable

    detached.type(Binary);
    EXPECT_EQ(detached.type(), Binary); // should have changed the detached type
    EXPECT_NE(x.type(), Binary); // should not have changed core's type
}

TEST(modeling_variables, detached_variable_remains_linked_to_its_core_even_when_indirected_by_a_regular_variable) {
    Environment env;
    Variable x = Variable(env, "x");
    DetachedVariable detached = DetachedVariable(x);
    Variable z = Variable(detached);
    z.value(100);
    EXPECT_EQ(z.value(), 100); // should have changed z's value
    EXPECT_EQ(detached.value(), 100); // should have changed detached's value
    EXPECT_EQ(x.value(), 0); // should not have changed x
    detached.update_core_value();
    EXPECT_EQ(x.value(), 100); // should have update its core value
}
