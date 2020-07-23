//
// Created by tim on 11.07.20.
//

#include <gtest/gtest.h>
#include <memory>

#include "solver.h"
#include "TestVar.h"


TEST(solver_test, remove_inconsistent) {
    auto a = std::make_shared<TestVar>(std::list{1, 4, 7});
    auto b = std::make_shared<TestVar>(std::list{0, 2, 5});
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

TEST(solver_test, create_incoming_arcs) {
    using TestVarPtr = std::shared_ptr<TestVar>;
    auto a = std::make_shared<TestVar>(std::list{1, 4, 7});
    auto b = std::make_shared<TestVar>(std::list{0, 2, 5});
    auto c = std::make_shared<TestVar>(std::list{2, 6, 7});
    a->setNeighbours({{b, std::less<>()}, {c, std::greater<>()}});
    std::vector<TestArc> arcs = csp::createIncomingArcs<int, IntComparator>(a);
    std::set<TestVarPtr> vars = {b, c};
    for (const auto &arc : arcs) {
        EXPECT_EQ(arc.to, a);
        auto it = vars.find(arc.from);
        if (it == vars.end()) {
            FAIL();
        } else {
            vars.erase(it);
        }
    }

    EXPECT_TRUE(vars.empty());
}