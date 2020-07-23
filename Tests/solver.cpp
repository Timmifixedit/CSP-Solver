//
// Created by tim on 11.07.20.
//

#include <gtest/gtest.h>
#include <memory>

#include "solver.h"
#include "TestVar.h"

using TestArc = csp::Arc<int, IntComparator>;

TEST(solver_test, remove_inconsistent) {
    auto a = std::make_shared<TestVar>("a", std::list{1, 4, 7});
    auto b = std::make_shared<TestVar>("a", std::list{0, 2, 5});
    TestArc aLessB(a, b, std::less<>());
    TestArc bGreaterA(b, a, std::greater<>());

    csp::removeInconsistent(aLessB);
    EXPECT_EQ(aLessB.from->valueDomain().size(), 2);
    EXPECT_EQ(aLessB.from->valueDomain().front(), 1);
    EXPECT_EQ(aLessB.from->valueDomain().back(), 4);

    csp::removeInconsistent(bGreaterA);
    EXPECT_EQ(bGreaterA.from->valueDomain().size(), 2);
    EXPECT_EQ(bGreaterA.from->valueDomain().front(), 2);
    EXPECT_EQ(bGreaterA.from->valueDomain().back(), 5);
}
