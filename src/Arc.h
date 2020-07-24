//
// Created by tim on 22.07.20.
//

#ifndef CSP_SOLVER_ARC_H
#define CSP_SOLVER_ARC_H

#include <type_traits>
#include <vector>
#include <cassert>

#include "Variable.h"

namespace csp {

    template<typename T, typename Predicate>
    class Constraint {
        static_assert(std::is_invocable_r_v<bool, Predicate, T, T>,
                      "Constraint predicate has to map from (T, T) -> bool");
    public:
        Constraint(VarPtr<T> v1, VarPtr<T> v2, Predicate predicate) : var1(std::move(v1)),
            var2(std::move(v2)), predicate(std::move(predicate)) {}
    protected:
        const VarPtr<T> var1, var2;
        const Predicate predicate;
    };

    template<typename T, typename Predicate>
    class Arc : public Constraint<T, Predicate> {
    public:
        Arc(VarPtr<T> v1, VarPtr<T> v2, Predicate predicate, bool reverse = false) :
            Constraint<T, Predicate>(std::move(v1), std::move(v2), std::move(predicate)), reversed(reverse) {}

        void reverse() noexcept {
            reversed = !reversed;
        }

        VarPtr<T> from() const noexcept {
            return reversed ? this->var2 : this->var1;
        }

        cVarPtr<T> to() const noexcept {
            return reversed ? this->var1 : this->var2;
        }

        bool constraintSatisfied(const T &valFrom, const T &valTo)
            const noexcept(noexcept(std::declval<Predicate>()(std::declval<T>(), std::declval<T>()))) {
            return reversed ? this->predicate(valTo, valFrom) : this->predicate(valFrom, valTo);
        }

    private:
        bool reversed;
    };
}


#endif //CSP_SOLVER_ARC_H
