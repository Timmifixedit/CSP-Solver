/**
 * @author Tim Luchterhand
 * @date 22.07.20
 * @brief This file contains the the csp::Arc and the csp::Constraint class. Both can be used to specify constraints
 * (dependencies) between two csp::Variable pointers. An Arc is directed (specifying a constraint that a Variable X
 * X imposes on a Variable Y, e.g. A < B but not B > A). A Constraint implicitly defines both directions
 * (e.g. if A < B <=> B > A). The actual constraint is given as a binary predicate.
 */

#ifndef CSP_SOLVER_ARC_H
#define CSP_SOLVER_ARC_H

#include <type_traits>
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

        /**
         * Used to check if type can be dereferenced using *-Operator
         * @tparam T Type to be checked
         */
        template<typename T>
        struct is_dereferencable : decltype(implementations::pointerTest<T>(std::declval<T>())) {};

        /**
         * Used to check if type is derived from csp::Variable
         * @tparam T Type to be checked
         */
        template<typename T>
        struct is_derived_from_var : decltype(implementations::derivedTest(std::declval<std::remove_reference_t<T>*>())) {};
    }


    template<typename T>
    using BinaryPredicate = std::function<bool(const T&, const T&)>;

    template<typename VarPtr>
    class Arc;

    /**
     * Represents a binary undirected constraint in a constraint satisfaction problem
     * @tparam VarPtr Pointer-type to a type derived from csp::Variable
     */
    template<typename VarPtr>
    class Constraint {
    public:
        static_assert(type_traits::is_dereferencable<VarPtr>::value,
                      "Constraints must be constructed from pointer type to csp::Variable");
        static_assert(type_traits::is_derived_from_var<decltype(*std::declval<VarPtr>())>::value,
                      "Type referenced by VarPtr must derive from csp::Variable");
        using VarType = typename std::remove_reference_t<decltype(*std::declval<VarPtr>())>::ValueT;

        /**
         * Ctor
         * @param v1 Pointer-Type to first variable
         * @param v2 Pointer-Type to first variable
         * @param predicate Constraint in form of a binary predicate
         */
        Constraint(VarPtr v1, VarPtr v2, BinaryPredicate<VarType> predicate) : var1(std::move(v1)), var2(std::move(v2)),
        predicate(std::move(predicate)) {}

        using ArcT = Arc<VarPtr>;

        /**
         * Create the two equivalent directed csp::Arcs
         * @return Pair of equivalent csp::Arcs
         */
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

    /**
     * Represents a binary constraint as directed arc in a constraint satisfaction problem. Is mainly used during
     * solving to obtain arc consistency
     * @tparam VarPtr Pointer-type to a type derived from csp::Variable
     * @note Constraints implicitly specify two directed arcs. For example: A constraint A < B is equivalent to
     * two arcs A < B and B > A
     */
    template<typename VarPtr>
    class Arc : public Constraint<VarPtr> {
    public:
        using VarType = typename Constraint<VarPtr>::VarType;
        /**
         * Ctor
         * @param v1 Pointer-Type to first variable
         * @param v2 Pointer-Type to first variable
         * @param predicate Constraint in form of a binary predicate
         * @param reverse Specifies whether the arc represents v2 -> v1 instead
         */
        Arc(VarPtr v1, VarPtr v2, BinaryPredicate<VarType> predicate, bool reverse = false) :
                Constraint<VarPtr>(std::move(v1), std::move(v2), std::move(predicate)), reversed(reverse) {}

        /**
         * Reverses the arc (switches from() and to() members)
         */
        void reverse() noexcept {
            reversed = !reversed;
        }

        /**
         * Gets the source node of the arc
         * @return Always returns the pointer to the source node of the arc, taking into account if the arc is reversed
         */
        VarPtr from() const noexcept {
            return reversed ? this->var2 : this->var1;
        }

        /**
         * Gets the destination node of the arc
         * @return Always returns the pointer to the destination node of the arc, taking into account if the arc is
         * reversed
         */
        VarPtr to() const noexcept {
            return reversed ? this->var1 : this->var2;
        }

        /**
         * Checks if the binary constraint between source and destination is satisfied. If values are chosen from the
         * domains of from() and to() respectively, makes sure the constraint predicate is evaluated correctly
         * @param valFrom value of the source node
         * @param valTo value of the destination node
         * @return true if constraint is satisfied, false otherwise
         */
        bool constraintSatisfied(const VarType &valFrom, const VarType &valTo) const noexcept(
                noexcept(std::declval<BinaryPredicate<VarType>>()(valFrom, valTo))) {
            return reversed ? this->predicate(valTo, valFrom) : this->predicate(valFrom, valTo);
        }

    private:
        bool reversed;
    };
}


#endif //CSP_SOLVER_ARC_H
