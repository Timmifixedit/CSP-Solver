//
// Created by tim on 23.07.20.
//

#ifndef CSP_SOLVER_TESTVAR_H
#define CSP_SOLVER_TESTVAR_H

#include <functional>

#include "Variable.h"
#include "Arc.h"

using IntComparator = std::function<bool(int, int)>;
using TestVar = csp::Variable<int, IntComparator>;
using TestArc = csp::Arc<int, IntComparator>;

#endif //CSP_SOLVER_TESTVAR_H
