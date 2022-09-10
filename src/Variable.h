/**
 * @author Tim Luchterhand
 * @date 11.07.20
 * @brief This file contains the definition of a variable which is used to describe a CSP. Custom variable types can
 * be used by deriving from csp::Variable
 */

#ifndef CSP_SOLVER_VARIABLE_H
#define CSP_SOLVER_VARIABLE_H
#include <list>
#include <memory>

namespace csp {

    /**
     * Represents a variable in a constraint satisfaction problem
     * @tparam T Type of the contained value
     */
    template<typename T, template<typename...> typename DomainType = std::list>
    class Variable {
    public:
        using DomainT = DomainType<T>;
        using ValueT = T;
        /**
         * CTor
         * @param domain list of possible values
         */
        explicit Variable(DomainT domain) : domain(std::move(domain)) {}

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
         * Sets the value domain accepting arbitrary containers that support iteration
         * @tparam Container Type of the container
         * @param container desired values
         * @note Avoid duplicates! No checks are performed. Duplicates can lead to performance losses.
         */
        template<typename Container, std::enable_if_t<
                std::is_convertible_v<decltype(*std::begin(std::declval<Container>())), T>, int> = 0>
        void setValueDomain (const Container &container) {
            domain.clear();
            std::copy(std::begin(container), std::end(container), std::back_inserter(domain));
        }

        /**
         * Sets the value domain
         * @param values desired  values.
         * @note Avoid duplicates! No checks are performed. Duplicates can lead to performance losses.
         */
        void setValueDomain(DomainT values) {
            domain = std::move(values);
        }

        /**
         * Gets the value domain
         * @return
         */
        [[nodiscard]] auto valueDomain() const noexcept -> const DomainT& {
            return domain;
        }

        /**
         * Gets the value domain
         * @return
         */
        [[nodiscard]] auto valueDomain() noexcept -> DomainT & {
            return domain;
        }
    private:
        DomainT domain;
    };
}

#endif //CSP_SOLVER_VARIABLE_H
