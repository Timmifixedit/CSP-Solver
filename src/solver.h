//
// Created by tim on 11.07.20.
//

#ifndef CSP_SOLVER_SOLVER_H
#define CSP_SOLVER_SOLVER_H
#include <cassert>

#include "Variable.h"
namespace csp {
    template<typename T, typename Derived>
    class MrvStrategy {
        bool operator() (const Variable<T, Derived> &lhs, const Variable<T, Derived> &rhs) const noexcept {
            return lhs.valueDomain().size() < rhs.valueDomain().size();
        }
    };

    template<typename T, typename Derived>
    bool removeInconsistent(Variable<T, Derived> &x, const Variable<T, Derived> &y) {
        assert(!x.isAssigned() && !y.isAssigned());
        bool removed = false;
        for (auto it = x.valueDomain().begin(); it != x.valueDomain().end();) {
            bool consistent = false;
            x.assign(*it);
            for (const auto &val : y.valueDomain()) {
                if (y.assignmentValid(val)) {
                    consistent = true;
                    break;
                }
            }

            if (!consistent) {
                it = x.valueDomain().erase(it);
                removed = true;
            } else {
                ++it;
            }
        }

        x.clearAssignment();
        return removed;
    }
}

#endif //CSP_SOLVER_SOLVER_H
