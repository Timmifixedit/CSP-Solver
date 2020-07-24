//
// Created by tim on 11.07.20.
//

#ifndef CSP_SOLVER_SOLVER_H
#define CSP_SOLVER_SOLVER_H
#include <unordered_map>
#include <vector>
#include <list>

#include "Variable.h"
#include "Arc.h"

namespace csp::util {

    template<typename T, typename Predicate>
    bool removeInconsistent(const Arc<T, Predicate> &arc) {
        bool removed = false;
        VarPtr<T> from = arc.from();
        cVarPtr<T> to = arc.to();
        for (auto it = from->valueDomain().begin(); it != from->valueDomain().end();) {
            bool consistent = false;
            for (const auto &val : to->valueDomain()) {
                if (arc.constraintSatisfied(*it, val)) {
                    consistent = true;
                    break;
                }
            }

            if (!consistent) {
                it = from->valueDomain().erase(it);
                removed = true;
            } else {
                ++it;
            }
        }

        return removed;
    }

    template<typename T, typename Predicate>
    struct Csp {
        using ArcT = Arc<T, Predicate>;
        Csp() = default;
        Csp(std::list<ArcT> arcs, std::unordered_map<VarPtr<T>, std::vector<ArcT>> incomingNeighbours) :
            arcs(std::move(arcs)), incomingNeighbours(std::move(incomingNeighbours)) {}
        std::list<ArcT> arcs;
        std::unordered_map<cVarPtr<T>, std::vector<ArcT>> incomingNeighbours;
    };

    template<typename T, typename Predicate>
    auto createCsp(const std::list<Constraint<T, Predicate>> &constraints) -> Csp<T, Predicate> {
        Csp<T, Predicate> ret;
        for (const auto &constraint : constraints) {
            auto [normal, reversed] = constraint.getArcs();
            ret.arcs.push_back(normal);
            ret.arcs.push_back(reversed);
            ret.incomingNeighbours[normal.from()].emplace_back(std::move(reversed));
            ret.incomingNeighbours[normal.to()].emplace_back(std::move(normal));
        }

        return ret;
    }

    template<typename T, typename Predicate>
    bool ac3(const Csp<T, Predicate> &problem) {
        using ArcT = Arc<T, Predicate>;
        std::list<ArcT> arcs = problem.arcs;
        while (!arcs.empty()) {
            const ArcT current = arcs.front();
            arcs.pop_front();
            if(removeInconsistent(current)) {
                if (current.from()->valueDomain().empty()) {
                    return false;
                }

                auto it = problem.incomingNeighbours.find(current.from());
                assert(it != problem.incomingNeighbours.end());
                std::copy_if(it->second.begin(), it->second.end(), std::back_inserter(arcs), [&current](const auto &arc) {
                    return arc.from() != current.to();
                });
            }
        }

        return true;
    }
}

#endif //CSP_SOLVER_SOLVER_H
