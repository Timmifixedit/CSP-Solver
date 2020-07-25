//
// Created by tim on 25.07.20.
//

#ifndef CSP_SOLVER_CSP_H
#define CSP_SOLVER_CSP_H

#include <unordered_map>

#include "Variable.h"
#include "Arc.h"

namespace csp {


    template<typename T>
    struct Csp {
        using ArcT = Arc<T>;
        std::vector<VarPtr < T>> variables;
        std::list<ArcT> arcs;
        std::unordered_map<cVarPtr < T>, std::vector<ArcT>> incomingNeighbours;
    };

    template<typename VarContainer, typename ArcContainer>
    auto make_csp(VarContainer &variables, ArcContainer &arcs) -> Csp<std::remove_reference_t<
            decltype(
            std::begin(variables),
                    std::end(variables),
                    std::size(variables),
                    std::begin(arcs),
                    std::end(arcs),
                    arcs.front().reverse(),
                    variables.front()->valueDomain().front()
            )>> {

        using VarT = std::remove_reference_t<decltype(variables.front()->valueDomain().front())>;
        Csp<VarT> ret;
        ret.variables.reserve(std::size(variables));
        for (const auto &v : variables) {
            ret.variables.emplace_back(v);
        }

        for (const auto &a : arcs) {
            ret.arcs.emplace_back(a);
        }

//        std::copy(std::begin(variables), std::end(variables), ret.variables.begin());
//        std::copy(std::begin(arcs), std::end(arcs), ret.arcs.begin());
        for (const auto &arc : ret.arcs) {
            ret.incomingNeighbours[arc.to()].emplace_back(arc);
        }

        return ret;
    }

    template<typename T>
    auto make_csp(std::vector<VarPtr < T>> variables, const std::list<Constraint < T>> &constraints) -> Csp<T> {
            std::list<Arc < T>> arcs;
            for (const auto &constraint : constraints) {
                auto[normal, reverse] = constraint.getArcs();
                arcs.emplace_back(std::move(normal));
                arcs.emplace_back(std::move(reverse));
        }

        return make_csp(variables, arcs);
    }
}

#endif //CSP_SOLVER_CSP_H
