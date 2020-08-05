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

TEST(util_test, ac3) {
    using namespace csp;
    using namespace util;
    auto varA = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varB = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varC = std::make_shared<TestVar>(std::list{2, 3, 1});
    TestConstraint aLessB(varA, varB, std::less<>());
    TestConstraint aLessC(varA, varC, std::less<>());
    TestConstraint bNotC(varB, varC, std::not_equal_to<>());
    Csp problem = make_csp(std::array{varA, varB, varC}, std::array{aLessB, aLessC, bNotC});
    EXPECT_TRUE(ac3(problem));
    EXPECT_EQ(varA->valueDomain(), (std::list{2, 1}));
    EXPECT_EQ(varB->valueDomain(), (std::list{2, 3}));
    EXPECT_EQ(varC->valueDomain(), (std::list{2, 3}));
}

TEST(util_test, ac3_unsolvable) {
    using namespace csp;
    using namespace util;
    auto varA = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varB = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varC = std::make_shared<TestVar>(std::list{2, 3, 1});
    TestConstraint aLessB(varA, varB, std::less<>());
    TestConstraint bLessC(varB, varC, std::less<>());
    TestConstraint cLessA(varC, varA, std::less<>());
    Csp problem = make_csp(std::array{varA, varB, varC}, std::array{aLessB, bLessC, cLessA});
    EXPECT_FALSE(ac3(problem));
}

TEST(util_test, csp_checkpoint) {
    std::array vars = {std::make_shared<TestVar>(std::list{1, 2, 3}),
                       std::make_shared<TestVar>(std::list{4, 5, 6}),
                       std::make_shared<TestVar>(std::list{7, 8, 9})};
    auto problem = csp::make_csp(vars, std::vector<TestConstraint>{});
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