//
// Created by tim on 25.07.20.
//

#include <gtest/gtest.h>
#include <iostream>

#include "TestTypes.h"
#include "solver.h"

class NamedVar : public csp::Variable<int> {
public:
    NamedVar(std::list<int> domain, std::string name) : csp::Variable<int>(std::move(domain)), name(std::move(name)) {}
    const std::string name;
};

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

TEST(solver_test, backtrack) {
    auto varA = std::make_shared<NamedVar>(std::list{1, 2, 3}, "A");
    auto varB = std::make_shared<NamedVar>(std::list{1, 2, 3}, "B");
    auto varC = std::make_shared<NamedVar>(std::list{1, 2, 3}, "C");
    auto varD = std::make_shared<NamedVar>(std::list{3, 2, 1}, "D");
    csp::Constraint ab(varA, varB, std::not_equal_to<>());
    csp::Constraint ac(varA, varC, std::not_equal_to<>());
    csp::Constraint bc(varB, varC, std::not_equal_to<>());
    csp::Constraint bd(varB, varD, std::not_equal_to<>());
    csp::Constraint cd(varC, varD, std::not_equal_to<>());
    csp::Csp problem = csp::make_csp(std::array{varA, varB, varC, varD},
                                     std::array{ab, ac, bc, bd, cd});

    // Makes sure that Variable A is assigned first, then Variable C. Given the ordering of the values in C's domain
    // this forces the solve algorithm to backtrack twice until 1 is selected for C.
    auto aThenCStrat = [](const auto &csp) {
        for (const auto &var : csp.variables) {
            if (!var->isAssigned()) {
                if (var->name == "A") {
                    return var;
                }

                if(var->name == "D") {
                    return var;
                }

                return var;
            }
        }

        return csp.variables.front();
    };

    EXPECT_TRUE(csp::solve(problem, aThenCStrat));
    EXPECT_EQ(varA->valueDomain().front(), 1);
    EXPECT_EQ(varB->valueDomain().front(), 2);
    EXPECT_EQ(varC->valueDomain().front(), 3);
    EXPECT_EQ(varD->valueDomain().front(), 1);
}

TEST(solver_test, empty_problem) {
    csp::Csp problem = csp::make_csp(std::vector<VarPtr>{}, std::vector<TestArc>{});
    EXPECT_TRUE(csp::solve(problem));
}