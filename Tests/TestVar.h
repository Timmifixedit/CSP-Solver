//
// Created by tim on 23.07.20.
//

#ifndef CSP_SOLVER_TESTVAR_H
#define CSP_SOLVER_TESTVAR_H
#include <functional>
#include <string>

#include "Variable.h"

using IntComparator = std::function<bool(int, int)>;
class TestVar : public csp::Variable<int, IntComparator> {
public:
    explicit TestVar(std::string name);
    TestVar(std::string name, std::list<int> domain);

    const std::string name;
};

#endif //CSP_SOLVER_TESTVAR_H
