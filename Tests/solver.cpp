//
// Created by tim on 11.07.20.
//

#include <gtest/gtest.h>
#include <memory>

#include "solver.h"
#include "TestTypes.h"

TEST(solver_test, remove_inconsistent_values) {
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

TEST(solver_test, remove_inconsistent_indicate_removal) {
    using namespace csp::util;
    auto varA = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varB = std::make_shared<TestVar>(std::list{2, 3, 1});
    TestArc a(varA, varB, std::less<>());
    EXPECT_TRUE(removeInconsistent(a));
    EXPECT_FALSE(removeInconsistent(a));
}