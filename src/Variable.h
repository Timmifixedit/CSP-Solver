//
// Created by tim on 11.07.20.
//

#ifndef CSP_SOLVER_VARIABLE_H
#define CSP_SOLVER_VARIABLE_H
#include <optional>
#include <list>
#include <vector>
#include <memory>

namespace csp {

    template<typename T, typename Derived>
    class Variable;

    template<typename T, typename Derived>
    using VarPtr = std::shared_ptr<Variable<T, Derived>>;

    template<typename T, typename Derived>
    class Variable {
    public:
        Variable(std::optional<T> value, std::list<T> domain, std::vector<VarPtr<T, Derived>> dependencies) :
                value(std::move(value)),
                domain(std::move(domain)),
                deps(std::move(dependencies)) {}

        explicit Variable(std::list<T> domain) : Variable(std::nullopt, domain, {}) {}

        Variable(std::optional<T> value, std::list<T> domain) : Variable(value, domain, {}) {}

        [[nodiscard]] bool assignmentValid(const T &val) const {
            return static_cast<const Derived*>(this)->assignmentValid(val);
        }

        void assign(T val) noexcept {
            value.emplace(val);
        }

        void clearAssignment() noexcept {
            value.reset();
        }

        [[nodiscard]] bool isAssigned() const noexcept {
            return value.has_value();
        }

        [[nodiscard]] auto valueDomain() const noexcept -> const std::list<T>& {
            return domain;
        }

        [[nodiscard]] auto valueDomain() noexcept -> std::list<T>& {
            return domain;
        }

        void setDependencies(std::vector<VarPtr<T, Derived>> dependencies) noexcept {
            this->deps = std::move(dependencies);
        }

        auto getDependencies() const noexcept -> const std::vector<VarPtr<T, Derived>>& {
            return deps;
        }

        [[nodiscard]] auto getVal() const noexcept -> std::optional<T> {
            return value;
        }

    private:
        std::optional<T> value;
        std::list<T> domain;
        std::vector<VarPtr<T, Derived>> deps;
    };
}

#endif //CSP_SOLVER_VARIABLE_H
