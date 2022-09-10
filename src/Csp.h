/**
 * @file Csp.h
 * @author Tim Luchterhand
 * @date 25.07.20
 * @brief This file contains the csp::Csp class which represents a complete definition of a constraint satisfaction
 * problem. The class contains a list of Variable pointers, a list of all arcs and a map that specifies incoming arcs
 * of each variable. The csp::Csp should be created using the provided function csp::make_csp.
 */

#ifndef CSP_SOLVER_CSP_H
#define CSP_SOLVER_CSP_H

#include <unordered_map>
#include <vector>
#include <deque>

#include "Variable.h"
#include "Arc.h"

/**
 * @brief Contains all relevant datastructures and functions for defining and solving a constraint satisfaction problem
 */
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
        using VarT = std::remove_reference_t<decltype(*std::declval<VarPtr>())>;
        using VarListT = std::vector<VarPtr>;
        using ArcListT = std::deque<ArcT>;
        using NeighbourListT = std::unordered_map<VarPtr, std::vector<ArcT>>;
        const VarListT variables;
        const ArcListT arcs;
        const NeighbourListT incomingNeighbours;

        template<typename VarIt, typename ArcIt, std::enable_if_t<type_traits::is_arc<
                std::remove_reference_t<decltype(*std::declval<ArcIt>())>>::value, int>>
        friend auto make_csp(VarIt vBegin, VarIt vEnd, ArcIt aBegin, ArcIt aEnd)
        -> Csp<std::decay_t<decltype(*++aBegin, aBegin == aEnd, ++vBegin, vBegin == vEnd, *vBegin)>>;

    private:
        Csp(VarListT variables, ArcListT arcs, NeighbourListT neighbours) :
                variables(std::move(variables)), arcs(std::move(arcs)), incomingNeighbours(std::move(neighbours)) {}
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
    auto make_csp(const VarContainer &variables, const ArcContainer &arcs) -> Csp<std::decay_t<decltype(
                    std::end(arcs),
                    std::end(variables),
                    *std::begin(variables)
            )>> {
        return make_csp(std::begin(variables), std::end(variables), std::begin(arcs), std::end(arcs));
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
    auto make_csp(const VarContainer &variables, const ContraintContainer &constraints) -> Csp<std::decay_t<decltype(
                    std::end(constraints),
                    std::end(variables),
                    *std::begin(variables)
            )>> {
        return make_csp(std::begin(variables), std::end(variables), std::begin(constraints), std::end(constraints));
    }

    /**
     * Creates a CSP from a container of variable-pointers and a container of csp::Constraint using iterators. Variables
     * and constraints are taken from the respective range [begin, end)
     * @tparam VarIt Iterator type of variable container
     * @tparam ConstrIt Iterator type of constraint container
     * @param vBegin start of range of variables
     * @param vEnd end of range of variables (exclusive)
     * @param cBegin start of range of constraints
     * @param cEnd start of range of constraints (exclusive)
     * @return csp::Csp representing the problem induced by the given variables and constraints
     * @note When using csp::Constraints to specify the constraints, specify them only once. A csp::Constraint for
     * e.g. A < B fully represents the constraint between the csp::Variable A and B. Specifying A < B and B > A
     * may lead to performance loss during search!
     */
    template<typename VarIt, typename ConstrIt, std::enable_if_t<type_traits::is_constraint<
            std::remove_reference_t<decltype(*std::declval<ConstrIt>())>>::value, int> = 0>
    auto make_csp(VarIt vBegin, VarIt vEnd, ConstrIt cBegin, ConstrIt cEnd)
    -> Csp<std::decay_t<decltype(*++cBegin, cBegin == cEnd, ++vBegin, vBegin == vEnd, *vBegin)>> {
        using VarPtr = std::decay_t<decltype(*vBegin)>;
        using ArcT = typename Csp<VarPtr>::ArcT;
        std::vector<ArcT> arcs;
        while (cBegin != cEnd) {
            auto[normal, reverse] = cBegin->getArcs();
            arcs.emplace_back(std::move(normal));
            arcs.emplace_back(std::move(reverse));
            ++cBegin;
        }

        return make_csp(vBegin, vEnd, std::begin(arcs), std::end(arcs));

    }

    /**
     * Creates a CSP from a container of variable-pointers and a container of csp::Arc using iterators. Variables and
     * arcs are taken from the respective range [begin, end)
     * @tparam VarIt Iterator type of variable container
     * @tparam ArcIt Iterator type of arc container
     * @param vBegin start of range of variables
     * @param vEnd end of range of variables (exclusive)
     * @param aBegin start of range of arcs
     * @param aEnd start of range of arcs (exclusive)
     * @return csp::Csp representing the problem induced by the given variables and arcs
     * @note When using csp::Arcs to specify the constraints, make sure that if you have a constraint e.g. A < B,
     * you specify both csp::Arcs representing A < B and B > A! Otherwise the problem is malformed and may lead to
     * invalid solutions!
     */
    template<typename VarIt, typename ArcIt, std::enable_if_t<type_traits::is_arc<
            std::remove_reference_t<decltype(*std::declval<ArcIt>())>>::value, int> = 0>
    auto make_csp(VarIt vBegin, VarIt vEnd, ArcIt aBegin, ArcIt aEnd)
        -> Csp<std::decay_t<decltype(*++aBegin, aBegin == aEnd, ++vBegin, vBegin == vEnd, *vBegin)>> {
        using VarPtr = std::decay_t<decltype(*vBegin)>;
        using VarListT = typename Csp<VarPtr>::VarListT;
        using ArcListT = typename Csp<VarPtr>::ArcListT;
        using NeighbourListT = typename Csp<VarPtr>::NeighbourListT;
        VarListT vars;
        ArcListT cspArcs;
        NeighbourListT neighbours;
        std::copy(vBegin, vEnd, std::back_inserter(vars));
        std::copy(aBegin, aEnd, std::back_inserter(cspArcs));
        for (const auto &arc : cspArcs) {
            neighbours[arc.to()].emplace_back(arc);
        }

        return Csp<VarPtr>(std::move(vars), std::move(cspArcs), std::move(neighbours));
    }
}

#endif //CSP_SOLVER_CSP_H
