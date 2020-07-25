//
// Created by tim on 11.07.20.
//

#ifndef CSP_SOLVER_UTIL_H
#define CSP_SOLVER_UTIL_H
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
        std::vector<VarPtr<T>> variables;
        std::list<ArcT> arcs;
        std::unordered_map<cVarPtr<T>, std::vector<ArcT>> incomingNeighbours;
    };

    template<typename T, typename Predicate>
    auto createCsp(std::vector<VarPtr<T>> variables, std::list<Arc<T, Predicate>> arcs)
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
    auto createCsp(std::vector<VarPtr<T>> variables, const std::list<Constraint<T, Predicate>> &constraints)
        -> Csp<T, Predicate> {
        std::list<Arc<T, Predicate>> arcs;
        for (const auto &constraint : constraints) {
            auto [normal, reverse] = constraint.getArcs();
            arcs.emplace_back(std::move(normal));
            arcs.emplace_back(std::move(reverse));
        }

        return createCsp(std::move(variables), std::move(arcs));
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
                std::copy_if(it->second.begin(), it->second.end(), std::back_inserter(arcs),
                        [&current](const auto &arc) {return arc.from() != current.to();});
            }
        }

        return true;
    }

    template<typename T>
    using CspCheckpoint = std::vector<std::list<T>>;

    template<typename T, typename Predicate>
    auto makeCspCheckpoint(const Csp<T, Predicate> &problem) -> CspCheckpoint<T> {
        CspCheckpoint<T> ret;
        ret.reserve(problem.variables.size());
        for (const auto &var : problem.variables) {
            ret.emplace_back(var->valueDomain());
        }

        return ret;
    }

    template<typename T, typename Predicate>
    void restoreCspFromCheckpoint(const Csp<T, Predicate> &problem, const CspCheckpoint<T> &checkpoint) {
        assert(checkpoint.size() == problem.variables.size());
        for (std::size_t i = 0; i < checkpoint.size(); ++i) {
            problem.variables[i]->setValueDomain(checkpoint[i]);
        }
    }
}

#endif //CSP_SOLVER_UTIL_H
