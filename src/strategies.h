/**
 * @file strategies.h
 * @author Tim Luchterhand
 * @date 25.07.20
 * @brief This file contains different variable choosing strategies that can be used to solve a CSP. Apart from these
 * strategies, custom strategies can be used. A strategy must return one of the pointers to a variable stored in the
 * given CSP.
 */

#ifndef CSP_SOLVER_STRATEGIES_H
#define CSP_SOLVER_STRATEGIES_H

#include <algorithm>

#include "Csp.h"
#include "Variable.h"


namespace csp::strategies {
    /**
     * Minimum remaining values strategy for variable selection during search. Chooses the variable with the fewest
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

    /**
     * Variable selection strategy that simply chooses the next unassigned variable
     * @tparam VarPtr Pointer-type to a type derived from csp::Variable
     */
    template<typename VarPtr>
    struct First {
         VarPtr operator() (const Csp<VarPtr> &problem) const {
             assert(!problem.variables.empty());
             auto it = std::find_if(problem.variables.begin(), problem.variables.end(),
                     [](const auto &var) {return !var->isAssigned();});
             return it != problem.variables.end() ? *it : problem.variables.front();
        };
    };
}

#endif //CSP_SOLVER_STRATEGIES_H
