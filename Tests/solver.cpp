//
// Created by tim on 25.07.20.
//

#include <gtest/gtest.h>
#include <iostream>

#include "TestTypes.h"
#include "solver.h"

TEST(solver_test, simple_csp) {
    auto varA = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varB = std::make_shared<TestVar>(std::list{2, 3, 1});
    auto varC = std::make_shared<TestVar>(std::list{2, 3, 1});
    TestConstraint aLessB(varA, varB, std::less<>());
    TestConstraint aLessC(varA, varC, std::less<>());
    TestConstraint bNotC(varB, varC, std::not_equal_to<>());
    csp::Csp problem = csp::make_csp(std::array{varA, varB, varC},
                                     std::array{aLessB, aLessC, bNotC});
    bool success = csp::solve(problem);
    EXPECT_TRUE(success);
    EXPECT_TRUE(varA->isAssigned());
    EXPECT_TRUE(varB->isAssigned());
    EXPECT_TRUE(varC->isAssigned());
    auto [alb, bga] = aLessB.getArcs();
    EXPECT_TRUE(alb.constraintSatisfied(alb.from()->valueDomain().front(), alb.to()->valueDomain().front()));
    EXPECT_TRUE(bga.constraintSatisfied(bga.from()->valueDomain().front(), bga.to()->valueDomain().front()));
    auto [alc, cga] = aLessB.getArcs();
    EXPECT_TRUE(alc.constraintSatisfied(alc.from()->valueDomain().front(), alc.to()->valueDomain().front()));
    EXPECT_TRUE(cga.constraintSatisfied(cga.from()->valueDomain().front(), cga.to()->valueDomain().front()));
    auto [bnc, cnb] = aLessB.getArcs();
    EXPECT_TRUE(bnc.constraintSatisfied(bnc.from()->valueDomain().front(), bnc.to()->valueDomain().front()));
    EXPECT_TRUE(cnb.constraintSatisfied(cnb.from()->valueDomain().front(), cnb.to()->valueDomain().front()));
}

TEST(solver_test, impossible_csp) {
    auto varA = std::make_shared<TestVar>(std::list{1, 2});
    auto varB = std::make_shared<TestVar>(std::list{1, 2});
    auto varC = std::make_shared<TestVar>(std::list{1, 2});
    TestConstraint aNotB(varA, varB, std::not_equal_to<>());
    TestConstraint aNotC(varA, varC, std::not_equal_to<>());
    TestConstraint bNotC(varB, varC, std::not_equal_to<>());
    csp::Csp problem = csp::make_csp(std::array{varA, varB, varC}, std::array{aNotB, aNotC, bNotC});
    EXPECT_FALSE(csp::solve(problem));
}