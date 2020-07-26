//
// Created by tim on 25.07.20.
//

#ifndef CSP_SOLVER_STRATEGIES_H
#define CSP_SOLVER_STRATEGIES_H

#include <algorithm>

#include "Csp.h"
#include "Variable.h"


namespace csp::strategies {
    /**
     * Minimum remaining values strategy for variable selection during search. Chooses the varaible with the fewest
     * remaining possible values
     * @tparam VarPtr Pointer-type to a type derived from csp::Variable
     */
    template<typename VarPtr>
    struct Mrv {
        VarPtr operator() (const Csp<VarPtr> &problem) const {
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
