//
// Created by tim on 23.07.20.
//

#include "TestVar.h"

TestVar::TestVar(std::string name):
    csp::Variable<int, IntComparator>({1, 2, 3}), name(std::move(name)) {}

TestVar::TestVar(std::string name, std::list<int> domain) :
    csp::Variable<int, IntComparator>(std::move(domain)), name(std::move(name)) {}