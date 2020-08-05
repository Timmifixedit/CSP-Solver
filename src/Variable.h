/**
 * @author Tim Luchterhand
 * @date 11.07.20
 * @brief This file contains the definition of a variable which is used to describe a CSP. Custom variable types can
 * be used by deriving from csp::Variable
 */

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
        void assign(T val) {
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
        void setValueDomain(std::list<T> values) {
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

        [[nodiscard]] bool operator==(const Variable &other) const {
            return valueDomain() == other.valueDomain();
        }

    private:
        std::list<T> domain;
    };
}

#endif //CSP_SOLVER_VARIABLE_H
