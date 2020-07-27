//
// Created by tim on 25.07.20.
//

#ifndef CSP_SOLVER_CSP_H
#define CSP_SOLVER_CSP_H

#include <unordered_map>

#include "Variable.h"
#include "Arc.h"

namespace csp {

    namespace type_traits {
        namespace implementations {
            template<typename VarPtr>
            std::true_type arcTest(Arc<VarPtr>);

            std::false_type arcTest(...);

            template<typename VarPtr>
            std::true_type constraintTest(Constraint<VarPtr>);

            std::false_type constraintTest(...);
        }

        /**
         * Used to check if type is of template type csp::Arc
         * @tparam T Type to be checked
         */
        template<typename T>
        struct is_arc : decltype(implementations::arcTest(std::declval<T>())){};


        /**
         * Used to check if type is of template type csp::Constraint
         * @tparam T Type to be checked
         */
        template<typename T>
        struct is_constraint  {
            static constexpr bool value = decltype(implementations::constraintTest(std::declval<T>()))::value
                    && ! is_arc<T>::value;
        };
    }


    /**
     * Represents a constraint satisfaction problem (CSP)
     * @tparam VarPtr VarPtr Pointer-type to a type derived from csp::Variable
     */
    template<typename VarPtr>
    struct Csp {
        using ArcT = Arc<VarPtr>;
        std::vector<VarPtr> variables;
        std::list<ArcT> arcs;
        std::unordered_map<VarPtr, std::vector<ArcT>> incomingNeighbours;
    };

    /**
     * Creates a CSP from a container of variable-pointer and a container of csp::Arcs
     * @tparam VarContainer Container-Type containing pointer-types to a type derived of csp::Variable
     * @tparam ArcContainer Container-Type containing csp::Arcs
     * @param variables Container of all variables in the CSP
     * @param arcs Container of all directed csp::Arcs in the CSP
     * @return csp::Csp representing the problem induced by the given variables and arcs
     * @note When using csp::Arcs to specify the constraints, make sure that if you have a constraint e.g. A < B,
     * you specify both csp::Arcs representing A < B and B > A! Otherwise the problem is malformed and may lead to
     * invalid solutions!
     */
    template<typename VarContainer, typename ArcContainer, std::enable_if_t<
            type_traits::is_arc<std::remove_reference_t<decltype(*std::begin(std::declval<ArcContainer>()))>>::value,
            int> = 0>
    auto make_csp(const VarContainer &variables, const ArcContainer &arcs) -> Csp<std::remove_const_t<std::remove_reference_t<
            decltype(
                    std::end(arcs),
                    std::end(variables),
                    *std::begin(variables)
            )>>> {

        using VarPtr = std::remove_const_t<std::remove_reference_t<decltype(*std::begin(variables))>>;
        Csp<VarPtr> ret;
        std::copy(std::begin(variables), std::end(variables), std::back_inserter(ret.variables));
        std::copy(std::begin(arcs), std::end(arcs), std::back_inserter(ret.arcs));
        for (const auto &arc : ret.arcs) {
            ret.incomingNeighbours[arc.to()].emplace_back(arc);
        }

        return ret;
    }
    /**
     * Creates a CSP from a container of variable-pointers and a container of csp::Constraints
     * @tparam VarContainer Container-Type containing pointer-types to a type derived of csp::Variable
     * @tparam ArcContainer Container-Type containing csp::Constraints
     * @param variables Container of all variables in the CSP
     * @param arcs Container of all undirected csp::Constraints in the CSP
     * @return csp::Csp representing the problem induced by the given variables and constraints
     * @note When using csp::Constraints to specify the constraints, specify them only once. A csp::Constraint for
     * e.g. A < B fully represents the constraint between the csp::Variable A and B. Specifying A < B and B > A
     * may lead to performance loss during search!
     */
    template<typename VarContainer, typename ContraintContainer, std::enable_if_t<type_traits::is_constraint<
            std::remove_reference_t<decltype(*std::begin(std::declval<ContraintContainer>()))>>::value, int> = 0>
    auto make_csp(const VarContainer &variables, const ContraintContainer &constraints) -> Csp<std::remove_const_t<std::remove_reference_t<
            decltype(
                    std::end(constraints),
                    std::end(variables),
                    *std::begin(variables)
            )>>> {
        using VarPtr = std::remove_const_t<std::remove_reference_t<decltype(*std::begin(variables))>>;
        std::list<Arc<VarPtr>> arcs;
        for (const auto &constraint : constraints) {
            auto[normal, reverse] = constraint.getArcs();
            arcs.emplace_back(std::move(normal));
            arcs.emplace_back(std::move(reverse));
        }

        return make_csp(variables, arcs);
    }
}

#endif //CSP_SOLVER_CSP_H
