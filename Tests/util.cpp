//
// Created by tim on 11.07.20.
//

#include <gtest/gtest.h>
#include <memory>

#include "util.h"
#include "TestTypes.h"

TEST(util_test, remove_inconsistent_values) {
    using namespace csp::util;
    auto varA = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varB = std::make_shared<TestVar>(std::list{2, 3, 1});
    TestArc a(varA, varB, std::less<>());
    removeInconsistent(a);
    EXPECT_EQ(varA->valueDomain(), (std::list{2, 1}));
    EXPECT_EQ(varB->valueDomain(), (std::list{2, 3, 1}));
    a.reverse();
    removeInconsistent(a);
    EXPECT_EQ(varA->valueDomain(), (std::list{2, 1}));
    EXPECT_EQ(varB->valueDomain(), (std::list{2, 3}));
}

TEST(util_test, remove_inconsistent_indicate_removal) {
    using namespace csp::util;
    auto varA = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varB = std::make_shared<TestVar>(std::list{2, 3, 1});
    TestArc a(varA, varB, std::less<>());
    EXPECT_TRUE(removeInconsistent(a));
    EXPECT_FALSE(removeInconsistent(a));
}

TEST(util_test, create_csp_from_constraints) {
    using namespace csp::util;
    auto varA = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varB = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varC = std::make_shared<TestVar>(std::list{2, 3, 1});
    TestConstraint aLessB(varA, varB, std::less<>());
    TestConstraint aLessC(varA, varC, std::less<>());
    TestConstraint bNotC(varB, varC, [](int lhs, int rhs) {return lhs != rhs;});
    Csp problem = createCsp({varA, varB, varC}, std::list{aLessB, aLessC, bNotC});
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

TEST(util_test, ac3) {
    using namespace csp::util;
    auto varA = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varB = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varC = std::make_shared<TestVar>(std::list{2, 3, 1});
    TestConstraint aLessB(varA, varB, std::less<>());
    TestConstraint aLessC(varA, varC, std::less<>());
    TestConstraint bNotC(varB, varC, [](int lhs, int rhs) {return lhs != rhs;});
    Csp problem = createCsp({varA, varB, varC}, std::list{aLessB, aLessC, bNotC});
    EXPECT_TRUE(ac3(problem));
    EXPECT_EQ(varA->valueDomain(), (std::list{2, 1}));
    EXPECT_EQ(varB->valueDomain(), (std::list{2, 3}));
    EXPECT_EQ(varC->valueDomain(), (std::list{2, 3}));
}

TEST(util_test, ac3_unsolvable) {
    using namespace csp::util;
    auto varA = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varB = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varC = std::make_shared<TestVar>(std::list{2, 3, 1});
    TestConstraint aLessB(varA, varB, std::less<>());
    TestConstraint bLessC(varB, varC, std::less<>());
    TestConstraint cLessA(varC, varA, std::less<>());
    Csp problem = createCsp({varA, varB, varC}, std::list{aLessB, bLessC, cLessA});
    EXPECT_FALSE(ac3(problem));
}

TEST(util_test, csp_checkpoint) {
    std::vector<std::shared_ptr<TestVar>> vars = {std::make_shared<TestVar>(std::list{1, 2, 3}),
                                                  std::make_shared<TestVar>(std::list{4, 5, 6}),
                                                  std::make_shared<TestVar>(std::list{7, 8, 9})};
    auto problem = csp::util::createCsp(vars, std::list<TestConstraint>{});
    auto checkpoint = csp::util::makeCspCheckpoint(problem);
    EXPECT_EQ(checkpoint.size(), problem.variables.size());
    for (std::size_t i = 0; i < checkpoint.size(); ++i) {
        EXPECT_EQ(checkpoint[i], problem.variables[i]->valueDomain());
    }

    vars[0]->assign(1);
    vars[1]->assign(5);
    vars[2]->assign(8);
    csp::util::restoreCspFromCheckpoint(problem, checkpoint);
    EXPECT_EQ(checkpoint.size(), problem.variables.size());
    for (std::size_t i = 0; i < checkpoint.size(); ++i) {
        EXPECT_EQ(checkpoint[i], problem.variables[i]->valueDomain());
    }
}