#include <iostream>
#include <functional>
#include <memory>

#include "Variable.h"
#include "Arc.h"
#include "solver.h"

using IntComperator = std::function<bool(int, int)>;
using TestVar = csp::Variable<int>;
using TestArc = csp::Arc<int, IntComperator>;



int main() {
    auto a = std::make_shared<TestVar>(std::list{1, 2, 3});
    auto b = std::make_shared<TestVar>(std::list{1, 2, 3});
    TestArc alb(a, b, std::less<>());
    csp::removeInconsistent(alb);
    for (auto val : a->valueDomain()) {
        std::cout << val << " ";
    }

    std::cout << std::endl;
    alb.reverse();
    csp::removeInconsistent(alb);
    for (auto val : b->valueDomain()) {
        std::cout << val << " ";
    }

    std::cout << std::endl;
    return 0;
}
