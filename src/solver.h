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
    auto recursiveSolve(const Csp<VarType> &problem, const Strategy &strategy) -> std::optional<Csp<VarType>>{
        using VarContentT = typename Csp<VarType>::VarContentT;
        using VarPtr = typename Csp<VarType>::VarPtr;
        auto localCopy = problem.clone();
        VarPtr nextVar = strategy(localCopy);
        if (nextVar->isAssigned()) {
            return localCopy;
        }

        //Moving storage as it will be overwritten by assign() anyway
        std::list<VarContentT> valueDomain = std::move(nextVar->valueDomain());
        for (auto &val : valueDomain) {
            nextVar->assign(std::move(val));
            //auto cp = util::makeCspCheckpoint(problem);
            auto reducedProblem = util::ac3(localCopy);
            if (!reducedProblem.has_value()) {
//                util::restoreCspFromCheckpoint(problem, cp);
                continue;
            }

            auto subSolution = recursiveSolve(*reducedProblem, strategy);
            if (subSolution.has_value()) {
                return subSolution;
            }

            //util::restoreCspFromCheckpoint(problem, cp);
        }

        return {};
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
    auto solve(const Csp<VarType> &problem, const Strategy &strategy = Strategy()) -> std::optional<Csp<VarType>> {
        static_assert(std::is_invocable_r_v<typename Csp<VarType>::VarPtr, Strategy, Csp<VarType>>,
                "Invalid strategy object! Must map from csp::Csp -> VarPtr");
        auto reducedProblem = util::ac3(problem);
        if (!reducedProblem.has_value()) {
            return {};
        }

        return recursiveSolve(*reducedProblem, strategy);
    }
}

#endif //CSP_SOLVER_SOLVER_H
