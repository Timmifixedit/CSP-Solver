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

        template<typename T>
        struct is_arc : decltype(implementations::arcTest(std::declval<T>())){};

        template<typename T>
        struct is_constraint  {
            static constexpr bool value = decltype(implementations::constraintTest(std::declval<T>()))::value
                    && ! is_arc<T>::value;
        };
    }


    template<typename VarPtr>
    struct Csp {
        using ArcT = Arc<VarPtr>;
        std::vector<VarPtr> variables;
        std::list<ArcT> arcs;
        std::unordered_map<VarPtr, std::vector<ArcT>> incomingNeighbours;
    };

    template<typename VarContainer, typename ArcContainer, std::enable_if_t<type_traits::is_arc<std::remove_reference_t<decltype(std::declval<ArcContainer>().front())>>::value, int> = 0>
    auto make_csp(const VarContainer &variables, const ArcContainer &arcs) -> Csp<std::remove_const_t<std::remove_reference_t<
            decltype(
                    std::size(variables),
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

    template<typename VarContainer, typename ContraintContainer, std::enable_if_t<type_traits::is_constraint<std::remove_reference_t<decltype(std::declval<ContraintContainer>().front())>>::value, int> = 0>
    auto make_csp(const VarContainer &variables, const ContraintContainer &constraints) -> Csp<std::remove_const_t<std::remove_reference_t<
            decltype(
            std::size(variables),
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
