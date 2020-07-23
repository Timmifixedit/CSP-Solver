//
// Created by tim on 11.07.20.
//

#ifndef CSP_SOLVER_SOLVER_H
#define CSP_SOLVER_SOLVER_H
#include <queue>

#include "Variable.h"
#include "Arc.h"

namespace csp {
    template<typename T, typename Constraint>
    class MrvStrategy {
        bool operator() (const Variable<T, Constraint> &lhs, const Variable<T, Constraint> &rhs) const noexcept {
            return lhs.valueDomain().size() < rhs.valueDomain().size();
        }
    };

    template<typename T, typename Constraint>
    bool removeInconsistent(Arc<T, Constraint> &arc) {
        bool removed = false;
        VarPtr<T, Constraint> from = arc.from;
        cVarPtr<T, Constraint> to = arc.to;
        for (auto it = from->valueDomain().begin(); it != from->valueDomain().end();) {
            bool consistent = false;
            for (const auto &val : to->valueDomain()) {
                if (arc.constraint(*it, val)) {
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

    template<typename T, typename Constraint>
    void ac3(std::queue<Arc<T, Constraint>> &arcs) {
        using ArcT = Arc<T, Constraint>;
        while (!arcs.empty()) {
            ArcT current = std::move(arcs.front());
            arcs.pop();
            if (removeInconsistent(current)) {
                auto neighbourArcs = createIncomingArcs(current.from);
                for (auto &arc : neighbourArcs) {
                    arcs.emplace(std::move(arc));
                }
            }
        }
    }
}

#endif //CSP_SOLVER_SOLVER_H
