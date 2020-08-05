/**
 * @author Tim Luchterhand
 * @date 11.07.20
 * @brief This file contains utility functions used by the search algorithm.
 */

#ifndef CSP_SOLVER_UTIL_H
#define CSP_SOLVER_UTIL_H
#include <unordered_map>
#include <vector>
#include <list>

#include "Variable.h"
#include "Arc.h"
#include "Csp.h"

namespace csp::util {

    /**
     * Removes all inconsistent values from the source node of the given csp::Arc. As a result, the source node's
     * value domain only contains values for which a valid value in the domain of the destination node exists.
     * @tparam VarPtr Pointer-type to a type derived from csp::Variable
     * @param arc Arc to be processed
     * @return True if the value domain of the source node was modified, false otherwise
     */
    template<typename VarType>
    bool removeInconsistent(Arc<VarType> &arc) {
        bool removed = false;
        using VarPtr = typename Arc<VarType>::VarPtr;
        using cVarPtr = typename Arc<VarType>::cVarPtr;
        VarPtr from = arc.from();
        cVarPtr to = arc.to();
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

    /**
     * Obtains arc consistency in a CSP using the AC3-algorithm
     * @tparam VarPtr Pointer-type to a type derived from csp::Variable
     * @param problem The CSP to be processed
     * @return True if arc consistency was obtained, false not possible
     */
    template<typename VarType>
    auto ac3(const Csp<VarType> &problem) -> std::optional<Csp<VarType>>{
        auto localCopy = problem.clone();
        using ArcT = Arc<VarType>;
        std::list<ArcT> arcs = localCopy.arcs;
        while (!arcs.empty()) {
            ArcT current = arcs.front();
            arcs.pop_front();
            if(removeInconsistent(current)) {
                if (current.from()->valueDomain().empty()) {
                    return {};
                }

                auto it = localCopy.incomingNeighbours.find(current.from());
                assert(it != localCopy.incomingNeighbours.end());
                std::copy_if(it->second.begin(), it->second.end(), std::back_inserter(arcs),
                        [&current](const auto &arc) {return arc.from() != current.to();});
            }
        }

        return localCopy;
    }

    template<typename T>
    using CspCheckpoint = std::vector<std::list<T>>;

    /**
     * Backs up all value domains of all variables in a CSP
     * @tparam VarPtr Pointer-type to a type derived from csp::Variable
     * @param problem The CSP to be backed up
     * @return vector of csp::Variable domains (std::list). Domains are ordered according to the variables in the CSP
     */
    template<typename VarType>
    auto makeCspCheckpoint(const Csp<VarType> &problem) -> CspCheckpoint<typename Csp<VarType>::VarContentT> {
        using VarContentT = typename Csp<VarType>::VarContentT; //std::remove_reference_t<decltype(std::declval<VarPtr>()->valueDomain().front())>;
        CspCheckpoint<VarContentT> ret;
        ret.reserve(problem.variables.size());
        for (const auto &var : problem.variables) {
            ret.emplace_back(var->valueDomain());
        }

        return ret;
    }

    /**
     * Restores the value domains of all csp::Variables in a CSP from the given checkpoint
     * @tparam VarPtr Pointer-type to a type derived from csp::Variable
     * @param problem The CPS to be restored
     * @param checkpoint Checkpoint to load the value domains from
     */
    template<typename VarType>
    void restoreCspFromCheckpoint(const Csp<VarType> &problem,
                                  const CspCheckpoint<typename Csp<VarType>::VarContentT> &checkpoint) {
        assert(checkpoint.size() == problem.variables.size());
        for (std::size_t i = 0; i < checkpoint.size(); ++i) {
            problem.variables[i]->setValueDomain(std::move(checkpoint[i]));
        }
    }
}

#endif //CSP_SOLVER_UTIL_H
