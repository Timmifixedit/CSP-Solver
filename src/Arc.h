//
// Created by tim on 22.07.20.
//

#ifndef CSP_SOLVER_ARC_H
#define CSP_SOLVER_ARC_H

#include <type_traits>
#include <vector>
#include <cassert>
#include <utility>
#include <functional>

#include "Variable.h"

namespace csp {
    template<typename T>
    using BinaryPredicate = std::function<bool(T, T)>;

    template<typename T>
    class Arc;

    template<typename T>
    class Constraint {
    public:
        Constraint(VarPtr<T> v1, VarPtr<T> v2, BinaryPredicate<T> predicate) : var1(std::move(v1)),
                                                                                var2(std::move(v2)),
                                                                                predicate(std::move(predicate)) {}

        using ArcT = Arc<T>;

        auto getArcs() const noexcept -> std::pair<ArcT, ArcT> {
            ArcT normal(var1, var2, predicate);
            ArcT reversed = normal;
            reversed.reverse();
            return {normal, reversed};
        }

    protected:
        VarPtr <T> var1, var2;
        BinaryPredicate<T> predicate;
    };

    template<typename T>
    class Arc : public Constraint<T> {
    public:
        Arc(VarPtr <T> v1, VarPtr <T> v2, BinaryPredicate<T> predicate, bool reverse = false) :
                Constraint<T>(std::move(v1), std::move(v2), std::move(predicate)), reversed(reverse) {}

        void reverse() noexcept {
            reversed = !reversed;
        }

        VarPtr <T> from() const noexcept {
            return reversed ? this->var2 : this->var1;
        }

        cVarPtr <T> to() const noexcept {
            return reversed ? this->var1 : this->var2;
        }

        bool constraintSatisfied(const T &valFrom, const T &valTo) const {
            return reversed ? this->predicate(valTo, valFrom) : this->predicate(valFrom, valTo);
        }

    private:
        bool reversed;
    };

    template<typename VarPtr>
    auto make_constraint(VarPtr var1, VarPtr var2,
                         BinaryPredicate<std::remove_reference_t<decltype(var1->valueDomain().front())>> predicate)
    -> Constraint<std::remove_reference_t<decltype(var1->valueDomain().front())>> {
        using VarT = std::remove_reference_t<decltype(var1->valueDomain().front())>;
        return Constraint<VarT>(std::move(var1), std::move(var2), std::move(predicate));
    }
}


#endif //CSP_SOLVER_ARC_H
