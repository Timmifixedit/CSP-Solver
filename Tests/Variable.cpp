//
// Created by tim on 11.07.20.
//

#include <gtest/gtest.h>
#include <memory>

#include "TestVar.h"


TEST(variable_test, ctor) {
    TestVar a("A", {1, 2, 3, 4});
    int val = 1;
    for (int i : a.valueDomain()) {
        EXPECT_EQ(val++, i);
    }

    EXPECT_FALSE(a.isAssigned());
    EXPECT_TRUE(a.getNeighbours().empty());
    EXPECT_EQ(a.name, "A");
}

TEST(variable_test, neighbours) {
    auto a = std::make_shared<TestVar>("A");
    auto b = std::make_shared<TestVar>("B");
    a->addNeighbour({b, std::less<>()});
    EXPECT_EQ(a->getNeighbours().size(), 1);
    EXPECT_EQ(a->getNeighbours()[0].first.lock(), b);
    b->setNeighbours({{a, std::less<>()}});
    EXPECT_EQ(b->getNeighbours().size(), 1);
    EXPECT_EQ(b->getNeighbours()[0].first.lock(), a);
}

TEST(variable_test, assign) {
    TestVar a("A");
    a.assign(4);
    EXPECT_TRUE(a.isAssigned());
    EXPECT_EQ(a.valueDomain().size(), 1);
    EXPECT_EQ(a.valueDomain().front(), 4);
}
