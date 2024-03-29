/**
 * @file csp_solver.h
 * @author Tim Luchterhand
 * @date 25.07.20
 * @brief This file contains the solving algorithm for CSPs. It consists of a recursive backtracking search which
 * repeatedly uses the AC-3 algorithm to reduce the search space. Use the csp::solve wrapper method.
 */

#ifndef CSP_SOLVER_CSP_SOLVER_H
#define CSP_SOLVER_CSP_SOLVER_H

#include <type_traits>

#include "Variable.h"
#include "Arc.h"
#include "util.h"
#include "strategies.h"

namespace csp {
    namespace util {
        /**
         * Recursive backtracking search for csp::Csps. Prefer using the wrapper function csp::solve
         * @tparam VarPtr Pointer-type to a type derived from csp::Variable
         * @tparam Strategy Type of value selection strategy during search
         * @param problem CSP to be solved
         * @param strategy value selection strategy object used during during search
         * @return True if problem was solved, false otherwise
         */
        template<typename VarPtr, typename Strategy>
        bool recursiveSolve(Csp<VarPtr> &problem, const Strategy &strategy) {
            using Domain = typename Csp<VarPtr>::VarT::DomainT;
            VarPtr nextVar = strategy(problem);
            if (nextVar->isAssigned()) {
                return true;
            }

            //Moving storage as it will be overwritten by assign() anyway
            Domain valueDomain = std::move(nextVar->valueDomain());
            for (auto &val : valueDomain) {
                nextVar->assign(std::move(val));
                // Back up value domains of all variables. This is considerably faster than creating a deep copy of the
                // whole Csp
                auto cp = util::makeCspCheckpoint(problem);
                if (!util::ac3(problem)) {
                    util::restoreCspFromCheckpoint(problem, cp);
                    continue;
                }

                if (recursiveSolve(problem, strategy)) {
                    return true;
                }

                util::restoreCspFromCheckpoint(problem, cp);
            }

            return false;
        }

    }

    /**
     * Solves a CSP. If a solution exists, the value domains of each variable in the given Csp will be reduced to
     * exactly one value. If multiple solutions exist, it is unspecified which is found. If no solution exists, false
     * is returned but the value domains of the variables might still be altered
     * @tparam VarPtr Pointer-type to a type derived from csp::Variable
     * @tparam Strategy Type of value selection strategy during search (default: minimum remaining values strategy).
     * Has to provide ()-Operator and return VarPtr from given csp::Csp
     * @param problem CSP to be solved
     * @return True if problem was solved, false otherwise
     */
    template<typename VarPtr, typename Strategy = strategies::Mrv<VarPtr>>
    bool solve(Csp<VarPtr> &problem, const Strategy &strategy = Strategy()) {
        static_assert(std::is_invocable_r_v<VarPtr, Strategy, Csp<VarPtr>>,
                "Invalid strategy object! Must map from csp::Csp -> VarPtr");
        if (std::empty(problem.variables)) {
            return true;
        }

        if (!util::ac3(problem)) {
            return false;
        }

        return util::recursiveSolve(problem, strategy);
    }
}

#endif //CSP_SOLVER_CSP_SOLVER_H
