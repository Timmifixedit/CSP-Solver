//
// Created by tim on 25.07.20.
//

#ifndef CSP_SOLVER_SOLVER_H
#define CSP_SOLVER_SOLVER_H

#include <list>
#include <algorithm>

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
    template<typename VarPtr, typename Strategy>
    bool recursiveSolve(const Csp<VarPtr> &problem, const Strategy &strategy) {
        using VarType = std::remove_reference_t<decltype(std::declval<VarPtr>()->valueDomain().front())>;
        VarPtr nextVar = strategy(problem);
        if (nextVar->isAssigned()) {
            return true;
        }

        //Moving storage as it will be overwritten by assign() anyway
        std::list<VarType> valueDomain = std::move(nextVar->valueDomain());
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
    template<typename VarPtr, typename Strategy = strategies::Mrv<VarPtr>>
    bool solve(const Csp<VarPtr> &problem, const Strategy &strategy = Strategy()) {
        if (!util::ac3(problem)) {
            return false;
        }

        return recursiveSolve(problem, strategy);
    }
}

#endif //CSP_SOLVER_SOLVER_H
