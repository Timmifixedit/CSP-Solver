//
// Created by tim on 23.07.20.
//

#ifndef CSP_SOLVER_TESTTYPES_H
#define CSP_SOLVER_TESTTYPES_H

#include <functional>

#include "Variable.h"
#include "Arc.h"

using IntComparator = std::function<bool(int, int)>;
using TestVar = csp::Variable<int>;
using TestArc = csp::Arc<int, IntComparator>;

#endif //CSP_SOLVER_TESTTYPES_H
