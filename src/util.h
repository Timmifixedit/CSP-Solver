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
#include "Csp.h"

namespace csp::util {

    template<typename T>
    bool removeInconsistent(const Arc<T> &arc) {
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

    template<typename T>
    bool ac3(const Csp<T> &problem) {
        using ArcT = Arc<T>;
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

    template<typename T>
    auto makeCspCheckpoint(const Csp<T> &problem) -> CspCheckpoint<T> {
        CspCheckpoint<T> ret;
        ret.reserve(problem.variables.size());
        for (const auto &var : problem.variables) {
            ret.emplace_back(var->valueDomain());
        }

        return ret;
    }

    template<typename T>
    void restoreCspFromCheckpoint(const Csp<T> &problem, const CspCheckpoint<T> &checkpoint) {
        assert(checkpoint.size() == problem.variables.size());
        for (std::size_t i = 0; i < checkpoint.size(); ++i) {
            problem.variables[i]->setValueDomain(checkpoint[i]);
        }
    }
}

#endif //CSP_SOLVER_UTIL_H
