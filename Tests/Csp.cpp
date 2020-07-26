//
// Created by tim on 25.07.20.
//

#include <gtest/gtest.h>

#include "Csp.h"
#include "TestTypes.h"

TEST(util_test, create_csp_from_constraints) {
    using namespace csp;
    auto varA = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varB = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varC = std::make_shared<TestVar>(std::list{2, 3, 1});
    TestConstraint aLessB(varA, varB, std::less<>());
    TestConstraint aLessC(varA, varC, std::less<>());
    TestConstraint bNotC(varB, varC, [](int lhs, int rhs) {return lhs != rhs;});
    std::array vars{varA, varB, varC};
    std::array constraints{aLessB, aLessC, bNotC};
    Csp problem = make_csp(vars, constraints);
    EXPECT_EQ(problem.variables[0], varA);
    EXPECT_EQ(problem.variables[1], varB);
    EXPECT_EQ(problem.variables[2], varC);
    EXPECT_EQ(problem.arcs.size(), 6);

    auto testArcsPerVar = [&problem] (const std::shared_ptr<TestVar> &var) {
        const auto &incomingArcs = problem.incomingNeighbours[var];
        EXPECT_EQ(incomingArcs.size(), 2);
        EXPECT_NE(incomingArcs[0].from(), incomingArcs[1].from());
        for (const auto &arc : incomingArcs) {
            EXPECT_EQ(arc.to(), var);
            EXPECT_NE(arc.from(), var);
            auto it = std::find_if(problem.arcs.begin(), problem.arcs.end(), [&arc](const TestArc &t) {
                return arc.from() == t.from() && arc.to() == t.to();
            });
            EXPECT_NE(it, problem.arcs.end());
            problem.arcs.erase(it);
        }
    };

    testArcsPerVar(varA);
    testArcsPerVar(varB);
    testArcsPerVar(varC);
    EXPECT_TRUE(problem.arcs.empty());
}
