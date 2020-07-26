//
// Created by tim on 23.07.20.
//

#ifndef CSP_SOLVER_TESTTYPES_H
#define CSP_SOLVER_TESTTYPES_H

#include <memory>

#include "Variable.h"
#include "Arc.h"

using TestVar = csp::Variable<int>;
using VarPtr = std::shared_ptr<TestVar>;
using TestArc = csp::Arc<VarPtr>;
using TestConstraint = csp::Constraint<VarPtr>;

#endif //CSP_SOLVER_TESTTYPES_H
