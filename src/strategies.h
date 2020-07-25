//
// Created by tim on 25.07.20.
//

#ifndef CSP_SOLVER_STRATEGIES_H
#define CSP_SOLVER_STRATEGIES_H

#include <algorithm>

#include "util.h"
#include "Variable.h"


namespace csp::strategies {
    template<typename T, typename Predicate>
    struct Mrv {
        VarPtr<T> operator() (const util::Csp<T, Predicate> &problem) const {
            auto it = std::min_element(problem.variables.begin(), problem.variables.end(),
                    [](const auto &lhs, const auto &rhs) {
                return rhs->isAssigned() || (!lhs->isAssigned() &&
                lhs->valueDomain().size() < rhs->valueDomain().size());
            });

            assert(it != problem.variables.end());
            return *it;
        }
    };
}

#endif //CSP_SOLVER_STRATEGIES_H