#include <iostream>
#include <functional>
#include <memory>

#include "Variable.h"
#include "Arc.h"
#include "util.h"

using IntComperator = std::function<bool(int, int)>;
using TestVar = csp::Variable<int>;
using TestArc = csp::Arc<int, IntComperator>;
using TestConstraint = csp::Constraint<int, IntComperator>;

class Test : public csp::Variable<int> {
public:
    Test() : csp::Variable<int>({}) {}
    explicit Test(std::list<int> domain) : csp::Variable<int>(std::move(domain)) {}
    int i = 4;
};

int main() {
    auto test1 = std::make_shared<Test>(std::list{1, 2, 3});
    auto test2 = std::make_shared<Test>(std::list{1, 2, 3});
    TestConstraint testC(test1, test2, std::less<>());
    auto [normal, reversed] = testC.getArcs();
    normal.from()->assign(18);
    reversed.from()->assign(17);
    std::cout << test1->valueDomain().front() << " " << test2->valueDomain().front() << std::endl;
    TestArc testArc(test1, test2, std::less<>());
    csp::util::removeInconsistent(testArc);
    for (auto val : test1->valueDomain()) {
        std::cout << val << " ";
    }
    std::cout << std::endl << test1->i << std::endl;
    auto a = std::make_shared<TestVar>(std::list{1, 2, 3});
    auto b = std::make_shared<TestVar>(std::list{1, 2, 3});
    TestArc alb(a, b, std::less<>());
    csp::util::removeInconsistent(alb);
    for (auto val : a->valueDomain()) {
        std::cout << val << " ";
    }

    std::cout << std::endl;
    alb.reverse();
    csp::util::removeInconsistent(alb);
    for (auto val : b->valueDomain()) {
        std::cout << val << " ";
    }

    std::cout << std::endl;
    return 0;
}
