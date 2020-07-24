//
// Created by tim on 11.07.20.
//

#ifndef CSP_SOLVER_VARIABLE_H
#define CSP_SOLVER_VARIABLE_H
#include <list>
#include <vector>
#include <memory>

namespace csp {

    template<typename T>
    class Variable;

    template<typename T>
    using VarPtr = std::shared_ptr<Variable<T>>;

    template<typename T>
    using cVarPtr = std::shared_ptr<const Variable<T>>;

    template<typename T>
    class Variable {
    public:
        explicit Variable(std::list<T> domain) : domain(std::move(domain)) {}

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

    private:
        std::list<T> domain;
    };
}

#endif //CSP_SOLVER_VARIABLE_H
