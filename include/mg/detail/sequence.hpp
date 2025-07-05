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