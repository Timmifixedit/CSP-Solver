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

    namespace type_traits {
        namespace implementations {
            template<typename T>
            std::true_type pointerTest(decltype(*std::declval<T>(), std::declval<T>()));

            template<typename T>
            std::false_type pointerTest(...);

            template<typename VarType>
            std::true_type derivedTest(Variable<VarType> *);

            std::false_type derivedTest(...);
        }

        template<typename T>
        struct is_dereferencable : decltype(implementations::pointerTest<T>(std::declval<T>())) {};

        template<typename T>
        struct is_derived_from_var : decltype(implementations::derivedTest(std::declval<std::remove_reference_t<T>*>())) {};
    }


    template<typename T>
    using BinaryPredicate = std::function<bool(const T&, const T&)>;

    template<typename VarPtr>
    class Arc;

    template<typename VarPtr>
    class Constraint {
    public:
        static_assert(type_traits::is_dereferencable<VarPtr>::value,
                      "Constraints must be constructed from pointer type to csp::Variable");
        static_assert(type_traits::is_derived_from_var<decltype(*std::declval<VarPtr>())>::value,
                      "Type referenced by VarPtr must derive from csp::Variable");
        using VarType = std::remove_reference_t<decltype(std::declval<VarPtr>()->valueDomain().front())>;

        Constraint(VarPtr v1, VarPtr v2, BinaryPredicate<VarType> predicate) : var1(std::move(v1)), var2(std::move(v2)),
        predicate(std::move(predicate)) {}

        using ArcT = Arc<VarPtr>;

        auto getArcs() const noexcept -> std::pair<ArcT, ArcT> {
            ArcT normal(var1, var2, predicate);
            ArcT reversed = normal;
            reversed.reverse();
            return {normal, reversed};
        }

    protected:
        VarPtr var1, var2;
        BinaryPredicate<VarType> predicate;
    };

    template<typename VarPtr>
    class Arc : public Constraint<VarPtr> {
    public:
        using VarType = typename Constraint<VarPtr>::VarType;
        Arc(VarPtr v1, VarPtr v2, BinaryPredicate<VarType> predicate, bool reverse = false) :
                Constraint<VarPtr>(std::move(v1), std::move(v2), std::move(predicate)), reversed(reverse) {}

        void reverse() noexcept {
            reversed = !reversed;
        }

        VarPtr from() const noexcept {
            return reversed ? this->var2 : this->var1;
        }

        VarPtr to() const noexcept {
            return reversed ? this->var1 : this->var2;
        }

        bool constraintSatisfied(const VarType &valFrom, const VarType &valTo) const {
            return reversed ? this->predicate(valTo, valFrom) : this->predicate(valFrom, valTo);
        }

    private:
        bool reversed;
    };
}


#endif //CSP_SOLVER_ARC_H
