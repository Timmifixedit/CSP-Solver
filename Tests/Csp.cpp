//
// Created by tim on 25.07.20.
//

#include <gtest/gtest.h>

#include "Csp.h"
#include "TestTypes.h"

void verifyCsp(csp::Csp<VarPtr> &problem, const VarPtr& varA, const VarPtr& varB, const VarPtr& varC) {
    EXPECT_EQ(problem.variables[0], varA);
    EXPECT_EQ(problem.variables[1], varB);
    EXPECT_EQ(problem.variables[2], varC);
    EXPECT_EQ(problem.arcs.size(), 6);
    auto arcsCopy = problem.arcs;

    auto testArcsPerVar = [&problem, &arcsCopy] (const VarPtr &var) {
        const auto arcEntry = problem.incomingNeighbours.find(var);
        EXPECT_NE(arcEntry, problem.incomingNeighbours.end());
        const auto &incomingArcs = arcEntry->second;
        EXPECT_EQ(incomingArcs.size(), 2);
        EXPECT_NE(incomingArcs[0].from(), incomingArcs[1].from());
        for (const auto &arc : incomingArcs) {
            EXPECT_EQ(arc.to(), var);
            EXPECT_NE(arc.from(), var);
            auto it = std::find_if(arcsCopy.begin(), arcsCopy.end(), [&arc](const TestArc &t) {
                return arc.from() == t.from() && arc.to() == t.to();
            });
            EXPECT_NE(it, problem.arcs.end());
            arcsCopy.erase(it);
        }
    };

    testArcsPerVar(varA);
    testArcsPerVar(varB);
    testArcsPerVar(varC);
    EXPECT_TRUE(arcsCopy.empty());
}

TEST(csp_test, create_csp_from_constraints) {
    using namespace csp;
    auto varA = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varB = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varC = std::make_shared<TestVar>(std::list{2, 3, 1});
    TestConstraint aLessB(varA, varB, std::less<>());
    TestConstraint aLessC(varA, varC, std::less<>());
    TestConstraint bNotC(varB, varC, std::not_equal_to<>());
    Csp problem = make_csp(std::array{varA, varB, varC}, std::array{aLessB, aLessC, bNotC});
    verifyCsp(problem, varA, varB, varC);
}

TEST(csp_test, create_csp_from_arcs) {
    using namespace csp;
    auto varA = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varB = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varC = std::make_shared<TestVar>(std::list{2, 3, 1});
    TestArc aLessB(varA, varB, std::less<>());
    TestArc bGreaterA = aLessB;
    bGreaterA.reverse();
    TestArc aLessC(varA, varC, std::less<>());
    TestArc cGreaterA = aLessC;
    cGreaterA.reverse();
    TestArc bNotC(varB, varC, std::not_equal_to<>());
    TestArc cNotB = bNotC;
    cNotB.reverse();
    Csp problem = make_csp(std::array{varA, varB, varC},
            std::array{aLessB, bGreaterA, aLessC, cGreaterA, bNotC, cNotB});
    verifyCsp(problem, varA, varB, varC);

}
