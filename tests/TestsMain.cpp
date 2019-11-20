#include <iostream>
#include "gtest/gtest.h"
#include "modeling/VariableTests.h"
#include "modeling/ExpressionTests.h"
#include "algorithms/DirectLPSolver.h"
#include "algorithms/DirectMILPSolver.h"
#include "algorithms/MILPBranchAndBound.h"
#include "algorithms/DantzigWolfeDecomposition.h"
#include "modeling/DualAngularModelTest.h"

int main(int argc, char **argv) {
    Application::load_configuration_file("../../tests/test.cfg");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
