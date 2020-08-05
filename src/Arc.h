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

    template<typename VarType>
    class Arc;

    /**
     * Represents a binary undirected constraint in a constraint satisfaction problem
     * @tparam VarPtr Pointer-type to a type derived from csp::Variable
     */
    template<typename VarType>
    class Constraint {
    public:
//        static_assert(type_traits::is_dereferencable<VarPtr>::value,
//                      "Constraints must be constructed from pointer type to csp::Variable");
        static_assert(type_traits::is_derived_from_var<VarType>::value,
                      "Type referenced by VarPtr must derive from csp::Variable");
        using VarContentT = typename std::remove_reference_t<decltype(std::declval<VarType>().valueDomain())>::value_type;
        using VarPtr = std::shared_ptr<VarType>;
        using cVarPtr = std::shared_ptr<const VarType>;

        Constraint(VarPtr v1, VarPtr v2, BinaryPredicate<VarContentT> predicate) :
            var1(std::move(v1)), var2(std::move(v2)), predicate(std::move(predicate)) {}

        using ArcT = Arc<VarType>;

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
        BinaryPredicate<VarContentT> predicate;
    };

    /**
     * Represents a binary constraint as directed arc in a constraint satisfaction problem. Is mainly used during
     * solving to obtain arc consistency
     * @tparam VarPtr Pointer-type to a type derived from csp::Variable
     * @note Constraints implicitly specify two directed arcs. For example: A constraint A < B is equivalent to
     * two arcs A < B and B > A
     */
    template<typename VarType>
    class Arc : public Constraint<VarType> {
    public:
        using VarContentT = typename Constraint<VarType>::VarContentT;
        using VarPtr = typename Constraint<VarType>::VarPtr;
        using cVarPtr = typename Constraint<VarType>::cVarPtr;
        Arc(VarPtr v1, VarPtr v2, BinaryPredicate<VarContentT> predicate, bool reverse = false) :
            Constraint<VarType>(std::move(v1), std::move(v2), std::move(predicate)), reversed(reverse) {}

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
        cVarPtr to() const noexcept {
            return reversed ? this->var1 : this->var2;
        }

        /**
         * Checks if the binary constraint between source and destination is satisfied. If values are chosen from the
         * domains of from() and to() respectively, makes sure the constraint predicate is evaluated correctly
         * @param valFrom value of the source node
         * @param valTo value of the destination node
         * @return true if constraint is satisfied, false otherwise
         */
        bool constraintSatisfied(const VarContentT &valFrom, const VarContentT &valTo) const noexcept(
                noexcept(std::declval<BinaryPredicate<VarContentT>>()(valFrom, valTo))) {
            return reversed ? this->predicate(valTo, valFrom) : this->predicate(valFrom, valTo);
        }

        auto getPredicate() const noexcept -> const BinaryPredicate<VarContentT>& {
            return this->predicate;
        }

        [[nodiscard]] bool isReversed() const noexcept {
            return reversed;
        }

    private:
        bool reversed;
    };

    template<typename VarType>
    Arc<VarType> make_arc(typename Arc<VarType>::VarPtr v1, typename Arc<VarType>::VarPtr v2, BinaryPredicate<typename Arc<VarType>::VarContentT> pred) {
        return Arc<VarType>(v1, v2, pred);
    }
}


#endif //CSP_SOLVER_ARC_H
