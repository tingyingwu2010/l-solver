#include <iostream>
#include "gtest/gtest.h"
#include "modeling/VariableTests.h"
#include "modeling/ExpressionTests.h"
#include "algorithms/DirectLPSolver.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
