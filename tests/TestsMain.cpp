#include <iostream>
#include "gtest/gtest.h"
#include "modeling/VariableTests.h"
#include "modeling/ExpressionTests.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
