#include <iostream>
#include "gtest/gtest.h"
#include "modeling/VariableTests.h"
#include "modeling/ExpressionTests.h"
#include "algorithms/DirectLPSolver-Test.h"
#include "algorithms/DirectMILPSolver-Test.h"
#include "algorithms/MILPBranchAndBound-Test.h"
#include "algorithms/DantzigWolfeDecompositionSolver-Test.h"
#include "modeling/DualAngularModelTest.h"
#include "algorithms/DantzigWolfeBranchAndPrice-Test.h"

int main(int argc, char **argv) {
    Application::load_configuration_file("../../tests/test.cfg");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
