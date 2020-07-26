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

    template<typename VarPtr, typename Strategy = strategies::Mrv<VarPtr>>
    bool solve(const Csp<VarPtr> &problem) {
        Strategy strategy;
        if (!util::ac3(problem)) {
            return false;
        }

        return recursiveSolve(problem, strategy);
    }
}

#endif //CSP_SOLVER_SOLVER_H
