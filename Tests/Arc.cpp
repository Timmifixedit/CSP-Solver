//
// Created by tim on 24.07.20.
//

#include <gtest/gtest.h>
#include <memory>

#include "TestTypes.h"

TEST(arc_test, ctor) {
    auto varA = std::make_shared<TestVar>(std::list{1, 2, 3});
    auto varB = std::make_shared<TestVar>(std::list{1, 2, 3});
    TestArc a(varA, varB, std::less<>());
    EXPECT_EQ(a.from(), varA);
    EXPECT_EQ(a.to(), varB);
}

TEST(arc_test, ctor_reversed) {
    auto varA = std::make_shared<TestVar>(std::list{1, 2, 3});
    auto varB = std::make_shared<TestVar>(std::list{1, 2, 3});
    TestArc a(varA, varB, std::less<>(), true);
    EXPECT_EQ(a.from(), varB);
    EXPECT_EQ(a.to(), varA);
}

TEST(arc_test, reverse) {
    auto varA = std::make_shared<TestVar>(std::list{1, 2, 3});
    auto varB = std::make_shared<TestVar>(std::list{1, 2, 3});
    TestArc a(varA, varB, std::less<>());
    EXPECT_TRUE(a.constraintSatisfied(2, 3));
    EXPECT_FALSE(a.constraintSatisfied(3, 2));
    a.reverse();
    EXPECT_EQ(a.from(), varB);
    EXPECT_EQ(a.to(), varA);
    EXPECT_TRUE(a.constraintSatisfied(3, 2));
    EXPECT_FALSE(a.constraintSatisfied(2, 3));
}

TEST(arc_test, create_arcs_from_constraint) {
    auto varA = std::make_shared<TestVar>(std::list{1, 2, 3});
    auto varB = std::make_shared<TestVar>(std::list{1, 2, 3});
    TestConstraint test(varA, varB, std::less<>());
    auto [aLessB, bGreaterA] = test.getArcs();
    EXPECT_EQ(varA, aLessB.from());
    EXPECT_EQ(varB, aLessB.to());
    EXPECT_EQ(varA, bGreaterA.to());
    EXPECT_EQ(varB, bGreaterA.from());
}
