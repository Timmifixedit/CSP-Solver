//
// Created by tim on 11.07.20.
//

#include <gtest/gtest.h>
#include <memory>

#include "Variable.h"
class VarTest : public csp::Variable<int, VarTest> {
public:
    VarTest() : csp::Variable<int, VarTest>({1, 2, 3, 4, 5, 6, 7}) {}
    explicit VarTest(int val) : csp::Variable<int, VarTest>(val, {1, 2, 3, 4, 5, 6, 7}) {}
    [[nodiscard]] bool assignmentValid(int val) const {
        return getVal().has_value() && val % *getVal() == 0;
    }
};

TEST(variable_test, ctor) {
    VarTest a;
    int val = 1;
    for (int i : a.valueDomain()) {
        EXPECT_EQ(val++, i);
    }

    EXPECT_FALSE(a.isAssigned());
    EXPECT_TRUE(a.getDependencies().empty());
}

TEST(variable_test, assignment_valid) {
    using VarTestBase = csp::Variable<int, VarTest>;
    VarTest a(5);
    VarTest b(4);
    VarTest c;
    VarTestBase &aBase = a;
    VarTestBase &bBase = b;
    VarTestBase &cBase = c;
    EXPECT_TRUE(aBase.assignmentValid(10));
    EXPECT_TRUE(bBase.assignmentValid(12));
    EXPECT_FALSE(bBase.assignmentValid(5));
    EXPECT_FALSE(cBase.assignmentValid(2));
}

TEST(variable_test, assign) {
    VarTest a;
    a.assign(4);
    EXPECT_EQ(4, a.getVal());
    EXPECT_TRUE(a.isAssigned());
    a.clearAssignment();
    EXPECT_FALSE(a.isAssigned());
}

TEST(variable_test, dependencies) {
    auto a = std::make_shared<VarTest>();
    auto b = std::make_shared<VarTest>(4);
    auto c = std::make_shared<VarTest>(17);
    a->setDependencies({b, c});
    EXPECT_EQ(a->getDependencies().size(), 2);
    EXPECT_EQ(a->getDependencies()[0], b);
    EXPECT_EQ(a->getDependencies()[1], c);
}