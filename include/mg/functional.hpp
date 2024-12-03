#pragma once

#include "detail/iter_n.hpp"

#include <cstdlib>
#include <type_traits>
#include <utility>

namespace mg
{
    /// <summary>
    /// A perfectly forwarded functor wrapper around a callable which can be stored in a template.
    /// Supported examples would include function pointers, references, capture-less lambdas,
    /// constexpr functor objects, etc.
    /// </summary>
    /// <typeparam name="Fn">The callable value to wrap.</typeparam>
    template <auto Fn>
    struct func
    {
        template <typename... Ts>
        decltype(auto)
        operator()(Ts&&... p_ts)
            const
            noexcept(std::is_nothrow_invocable_r_v<
                        std::invoke_result_t<decltype(Fn), Ts&&...>,
                        decltype(Fn),
                        Ts&&...>)
        {
            return Fn(std::forward<Ts>(p_ts)...);
        }
    };

    /// <summary>
    /// Iterate over a parameter pack with a fixed stride size. The number of parameters must be
    /// evenly divisible by the stride size else there will be a compilation error. 0 is considered
    /// evenly divisible by any stride size.
    /// </summary>
    /// <typeparam name="Stride">The chunk or stride size over the parameter pack and for forwarding to</typeparam>
    /// <typeparam name="Fn">The type of the callable to apply the chunked parameters to.</typeparam>
    /// <typeparam name="...Ts">The types of the parameters.</typeparam>
    /// <param name="p_fn">The callable to apply.</param>
    /// <param name="...p_ts">The parameters to chunk.</param>
    template <std::size_t Stride, typename Fn, typename... Ts>
    void iter_n(Fn&& p_fn, Ts&&... p_ts)
    {
        static_assert(
            sizeof...(p_ts) % Stride == 0,
            "The number of parameters provided to iter_n must be a multiple of the stride parameter.");
        static_assert(
            detail::iter_stride_specialized<Stride>::value,
            "The stride size is not supported (probably meaning it is too large).");

        if constexpr (sizeof...(p_ts) == 0)
        {
            return;
        }

        detail::iter_n<Stride>{}(std::forward<Fn>(p_fn), std::forward<Ts>(p_ts)...);
    }
}