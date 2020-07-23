//
// Created by tim on 11.07.20.
//

#ifndef CSP_SOLVER_VARIABLE_H
#define CSP_SOLVER_VARIABLE_H
#include <list>
#include <vector>
#include <memory>

namespace csp {

    template<typename T, typename Constraint>
    class Variable;

    template<typename T, typename Constraint>
    using VarPtr = std::shared_ptr<Variable<T, Constraint>>;

    template<typename T, typename Constraint>
    using cVarPtr = std::shared_ptr<const Variable<T, Constraint>>;

    template<typename T, typename Constraint>
    using weakVarPtr = std::weak_ptr<Variable<T, Constraint>>;

    template<typename T, typename Constraint>
    using Neighbour = std::pair<weakVarPtr<T, Constraint>, Constraint>;

    template<typename T, typename Constraint>
    class Variable {
    public:
        Variable(std::list<T> domain, std::vector<Neighbour<T, Constraint>> neighbours) :
                domain(std::move(domain)), adjacent(std::move(neighbours)) {}

        explicit Variable(std::list<T> domain) : Variable(domain, {}) {}

        void assign(T val) noexcept {
            domain = {val};
        }

        [[nodiscard]] bool isAssigned() const noexcept {
            return domain.size() == 1;
        }

        [[nodiscard]] auto valueDomain() const noexcept -> const std::list<T>& {
            return domain;
        }

        [[nodiscard]] auto valueDomain() noexcept -> std::list<T>& {
            return domain;
        }

        void addNeighbour(Neighbour<T, Constraint> neighbour) {
            adjacent.emplace_back(std::move(neighbour));
        }

        void setNeighbours(std::vector<Neighbour<T, Constraint>> neighbours) {
            this->adjacent = std::move(neighbours);
        }

        [[nodiscard]] auto getNeighbours() const noexcept -> const std::vector<Neighbour<T, Constraint>> & {
            return adjacent;
        }

    private:
        std::list<T> domain;
        std::vector<Neighbour<T, Constraint>> adjacent;
    };
}

#endif //CSP_SOLVER_VARIABLE_H
