/**
 * @author Tim Luchterhand
 * @date 25.07.20
 * @brief This file contains the solving algorithm for CSPs. It consists of a recursive backtracking search which
 * repeatedly uses the AC-3 algorithm to reduce the search space. Use the csp::solve wrapper mehtod.
 */

#ifndef CSP_SOLVER_SOLVER_H
#define CSP_SOLVER_SOLVER_H

#include <list>
#include <algorithm>
#include <type_traits>

#include "Variable.h"
#include "Arc.h"
#include "util.h"
#include "strategies.h"

namespace csp {

    /**
     * Recursive backtracking search for csp::Csps. Prefer using the wrapper function csp::solve
     * @tparam VarPtr Pointer-type to a type derived from csp::Variable
     * @tparam Strategy Type of value selection strategy during search
     * @param problem CSP to be solved
     * @param strategy value selection strategy object used during during search
     * @return True if problem was solved, false otherwise
     */
    template<typename VarType, typename Strategy>
    bool recursiveSolve(const Csp<VarType> &problem, const Strategy &strategy) {
        using VarContentT = typename Csp<VarType>::VarContentT;
        using VarPtr = typename Csp<VarType>::VarPtr;
        VarPtr nextVar = strategy(problem);
        if (nextVar->isAssigned()) {
            return true;
        }

        //Moving storage as it will be overwritten by assign() anyway
        std::list<VarContentT> valueDomain = std::move(nextVar->valueDomain());
        for (auto &val : valueDomain) {
            nextVar->assign(std::move(val));
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

    /**
     * Solves a CSP
     * @tparam VarPtr Pointer-type to a type derived from csp::Variable
     * @tparam Strategy Type of value selection strategy during search (default: minimum remaining values strategy).
     * Has to provide ()-Operator and return VarPtr from given csp::Csp
     * @param problem CSP to be solved
     * @return True if problem was solved, false otherwise
     */
    template<typename VarType, typename Strategy = strategies::Mrv<VarType>>
    bool solve(const Csp<VarType> &problem, const Strategy &strategy = Strategy()) {
        static_assert(std::is_invocable_r_v<typename Csp<VarType>::VarPtr, Strategy, Csp<VarType>>,
                "Invalid strategy object! Must map from csp::Csp -> VarPtr");
        if (!util::ac3(problem)) {
            return false;
        }

        return recursiveSolve(problem, strategy);
    }
}

#endif //CSP_SOLVER_SOLVER_H
