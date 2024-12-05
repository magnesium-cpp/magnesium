#pragma once

#include <limits>
#include <stdexcept>
#include <type_traits>

namespace mg
{
    /// <summary>
    /// Power operation implemented in the whole number domain. The use case for this operation is
    /// for simple power operations that involve whole numbers only. This allows the implementation
    /// to be constexpr, and keeps the result in the same domain (i.e. negative integer exponents
    /// still result in a floating point result). Base could be allowed to be negative with these
    /// rules, but given the use case this seems unlikely to be required, and would make the
    /// definition more confusing.
    /// </summary>
    /// <typeparam name="T">The arithmetic type domain to operate in.</typeparam>
    /// <param name="p_base">The base of the operation.</param>
    /// <param name="p_power">The power of the operation.</param>
    /// <returns>The result of (base ^ power).</returns>
    template <typename T>
    constexpr T whole_pow(const T& p_base, const T& p_power)
    {
        static_assert(std::numeric_limits<T>::is_integer
            && !std::is_same_v<bool, T>,
            "The type provided must be an arithmetic non-bool type.");
        if (p_power < 0)
        {
            throw std::invalid_argument("The power must be a whole number.");
        }

        if (p_base < 0)
        {
            throw std::invalid_argument("The base must be a whole number.");
        }

        if (p_base == 0)
        {
            if (p_power == 0)
            {
                throw std::invalid_argument("Base and power cannot both be 0.");
            }

            return p_base;
        }

        if (p_power == 0)
        {
            return T{ 1 };
        }

        if (p_power == 1)
        {
            return p_base;
        }

        auto result = p_base;
        for (T i = 0; i < p_power - 1; ++i)
        {
            result *= p_base;
        }

        return result;
    }
}