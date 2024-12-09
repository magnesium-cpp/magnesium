#pragma once

#include <utility>

namespace mg::detail
{
    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <typeparam name="Min"></typeparam>
    /// <typeparam name="Max"></typeparam>
    /// <typeparam name="Stride"></typeparam>
    /// <typeparam name="...Ints"></typeparam>
    /// <returns></returns>
    template <typename T, T Min, T Max, T Stride, T... Ints>
    auto make_offset_sequence()
    {
        // TODO: make this a cool logarithmic implementation to avoid issues with the number of template instatiantions.
        // TODO: Consider how to properly handle things that are not integers or basic arithmetic types (and therefore
        // for which the Stride multiplication may not work) or properly restrict this to numeric types.
        // TODO: For that would need asserts and to properly check that sizeof...(Ints) can fit into T.

        constexpr T next = Min + (static_cast<T>(sizeof...(Ints)) * Stride);
        if constexpr (next >= Max)
        {
            return std::integer_sequence<T, Ints...>{};
        }
        else
        {
            return make_offset_sequence<T, Min, Max, Stride, Ints..., next>();
        }
    }
}