//
// Created by tim on 25.07.20.
//

#ifndef CSP_SOLVER_CSP_H
#define CSP_SOLVER_CSP_H

#include <unordered_map>

#include "Variable.h"
#include "Arc.h"

namespace csp {

    template<typename T, typename Predicate>
    struct Csp {
        using ArcT = Arc<T, Predicate>;
        std::vector<VarPtr<T>> variables;
        std::list<ArcT> arcs;
        std::unordered_map<cVarPtr<T>, std::vector<ArcT>> incomingNeighbours;
    };

    template<typename T, typename Predicate>
    auto make_csp(std::vector<VarPtr<T>> variables, std::list<Arc<T, Predicate>> arcs)
    -> Csp<T, Predicate> {
        Csp<T, Predicate> ret;
        ret.variables = std::move(variables);
        ret.arcs = std::move(arcs);
        for (const auto &arc : ret.arcs) {
            ret.incomingNeighbours[arc.to()].emplace_back(arc);
        }

        return ret;
    }

    template<typename T, typename Predicate>
    auto make_csp(std::vector<VarPtr<T>> variables, const std::list<Constraint<T, Predicate>> &constraints)
    -> Csp<T, Predicate> {
        std::list<Arc<T, Predicate>> arcs;
        for (const auto &constraint : constraints) {
            auto [normal, reverse] = constraint.getArcs();
            arcs.emplace_back(std::move(normal));
            arcs.emplace_back(std::move(reverse));
        }

        return make_csp(std::move(variables), std::move(arcs));
    }
}

#endif //CSP_SOLVER_CSP_H
