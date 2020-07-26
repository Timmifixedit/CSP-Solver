//
// Created by tim on 11.07.20.
//

#ifndef CSP_SOLVER_VARIABLE_H
#define CSP_SOLVER_VARIABLE_H
#include <list>
#include <vector>
#include <memory>

namespace csp {

    /**
     * Represents a variable in a constraint satisfaction problem
     * @tparam T Type of the contained value
     */
    template<typename T>
    class Variable {
    public:
        /**
         * CTor
         * @param domain list of possible values
         */
        explicit Variable(std::list<T> domain) : domain(std::move(domain)) {}

        /**
         * Sets the variable to the specified value (by reducing the value domain to said value)
         * @param val The desired value
         * @note The value does not have to be in the variable's value domain. No checks are performed.
         */
        void assign(T val) noexcept {
            domain = {std::move(val)};
        }

        /**
         * Checks if the variable is assigned
         * @return True if the variable's domain contains exactly one value, false otherwise
         */
        [[nodiscard]] bool isAssigned() const noexcept {
            return domain.size() == 1;
        }

        /**
         * Sets the value domain
         * @param values desired range of values.
         * @note Avoid duplicates! No checks are performed. Duplicates can lead to performance losses.
         */
        void setValueDomain(std::list<T> values) noexcept {
            domain = std::move(values);
        }

        /**
         * Gets the value domain
         * @return
         */
        [[nodiscard]] auto valueDomain() const noexcept -> const std::list<T>& {
            return domain;
        }

        /**
         * Gets the value domain
         * @return
         */
        [[nodiscard]] auto valueDomain() noexcept -> std::list<T>& {
            return domain;
        }

    private:
        std::list<T> domain;
    };
}

#endif //CSP_SOLVER_VARIABLE_H
