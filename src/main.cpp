#include <iostream>
#include <functional>

#include "Variable.h"
#include "Arc.h"
#include "solver.h"

using intComperator = std::function<bool(int, int)>;

class Test : public csp::Variable<int, intComperator> {
public:
    Test() : Variable({}) {}
};


int main() {
    auto t1 = std::make_shared<Test>();
    std::queue<csp::Arc<int, intComperator>> arcs;
    arcs.emplace(t1, t1, [](int, int){return true;});
    csp::ac3(arcs);
    return 0;
}

