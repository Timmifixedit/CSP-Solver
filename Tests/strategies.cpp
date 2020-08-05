//
// Created by tim on 27.07.20.
//

#include <gtest/gtest.h>
#include <memory>

#include "TestTypes.h"
#include "Csp.h"
#include "strategies.h"

TEST(strategies_test, mrv) {
    auto a = std::make_shared<TestVar>(std::list{1});
    auto b = std::make_shared<TestVar>(std::list{1, 2});
    auto c = std::make_shared<TestVar>(std::list{1, 2, 3});
    auto d = std::make_shared<TestVar>(std::list{1, 2, 3 , 4});
    auto problem = csp::make_csp(std::array{a, b, c, d}, std::vector<TestArc>{});
    auto next = csp::strategies::Mrv<TestVar>()(problem);
    EXPECT_EQ(next, b);
}

TEST(strategy_test, first) {
    auto a = std::make_shared<TestVar>(std::list{1});
    auto b = std::make_shared<TestVar>(std::list{1, 2});
    auto c = std::make_shared<TestVar>(std::list{1, 2, 3});
    auto d = std::make_shared<TestVar>(std::list{1, 2, 3 , 4});
    auto problem = csp::make_csp(std::array{a, c, d, b}, std::vector<TestArc>{});
    auto next = csp::strategies::First<TestVar>()(problem);
    EXPECT_EQ(next, c);
}