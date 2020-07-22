//
// Created by tim on 22.07.20.
//

#ifndef CSP_SOLVER_ARC_H
#define CSP_SOLVER_ARC_H

#include <type_traits>
#include <vector>

#include "Variable.h"

namespace csp {
    template<typename T, typename Constraint>
    class Arc {
    public:
        static_assert(std::is_invocable_r_v<bool, Constraint, T, T>,
                "Constraint function has to map from (T, T) -> bool");
        Arc(VarPtr<T, Constraint> from, cVarPtr<T, Constraint> to, Constraint constraint) :
            from(std::move(from)), to(std::move(to)), constraint(std::move(constraint)){}

        VarPtr<T, Constraint> from;
        cVarPtr<T, Constraint> to;
        const Constraint constraint;
    };

    template<typename T, typename Constraint>
    auto createArcs(const VarPtr<T, Constraint> &variable) -> std::vector<Arc<T, Constraint>> {
        std::vector<Arc<T, Constraint>> ret;
        for (const auto &neighbour : variable->getNeighbours()) {
            ret.emplace_back(neighbour.first, variable, neighbour.second);
        }

        return ret;
    }
}


#endif //CSP_SOLVER_ARC_H
