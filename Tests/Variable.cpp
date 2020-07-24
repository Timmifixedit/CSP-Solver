//
// Created by tim on 11.07.20.
//

#include <gtest/gtest.h>
#include <memory>

#include "TestTypes.h"

TEST(variable_test, ctor) {
    TestVar a({1, 2, 3, 4});
    int val = 1;
    for (int i : a.valueDomain()) {
        EXPECT_EQ(val++, i);
    }

    EXPECT_FALSE(a.isAssigned());
}

TEST(variable_test, assign) {
    TestVar a({1, 2, 3});
    a.assign(2);
    EXPECT_TRUE(a.isAssigned());
    EXPECT_EQ(a.valueDomain().size(), 1);
    EXPECT_EQ(a.valueDomain().front(), 2);
}
