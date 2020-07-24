//
// Created by tim on 11.07.20.
//

#ifndef CSP_SOLVER_SOLVER_H
#define CSP_SOLVER_SOLVER_H
#include <queue>

#include "Variable.h"
#include "Arc.h"

namespace csp::util {

    template<typename T, typename Predicate>
    bool removeInconsistent(const Arc<T, Predicate> &arc) {
        bool removed = false;
        VarPtr<T> from = arc.from();
        cVarPtr<T> to = arc.to();
        for (auto it = from->valueDomain().begin(); it != from->valueDomain().end();) {
            bool consistent = false;
            for (const auto &val : to->valueDomain()) {
                if (arc.constraintSatisfied(*it, val)) {
                    consistent = true;
                    break;
                }
            }

            if (!consistent) {
                it = from->valueDomain().erase(it);
                removed = true;
            } else {
                ++it;
            }
        }

        return removed;
    }
}

#endif //CSP_SOLVER_SOLVER_H
