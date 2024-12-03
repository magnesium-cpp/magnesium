#pragma once

#include <cstdlib>
#include <type_traits>
#include <utility>

namespace mg::detail
{
    /// <summary>
    /// Generic class which implements iterating over a parameter pack with a fixed stride size. It
    /// is to be specialized for different stride values. This must be implemented as a functor
    /// since partial template specialization is not supported for functions.
    /// </summary>
    /// <typeparam name="Stride">The stride size for chunking the parameter pack.</typeparam>
    template <std::size_t Stride>
    struct iter_n
    {
        /// <summary>
        /// Marker type on the unspecialized type to easily give a diagnosis on an unspecialized
        /// stride value being given.
        /// </summary>
        using unspecialized = void;
    };

    /// <summary>
    /// Iteration specialization for stride size of 1.
    /// </summary>
    template <>
    struct iter_n<1>
    {
        template <typename Fn, typename T1, typename... Ts>
        void operator()(Fn&& p_fn, T1&& p_t1, Ts&&... p_ts)
        {
            p_fn(std::forward<T1>(p_t1));

            if constexpr (sizeof...(p_ts) != 0)
            {
                (*this)(p_fn, std::forward<Ts>(p_ts)...);
            }
        }
    };

    /// <summary>
    /// Iteration specialization for stride size of 2.
    /// </summary>
    template <>
    struct iter_n<2>
    {
        template <typename Fn, typename T1, typename T2, typename... Ts>
        void operator()(Fn&& p_fn, T1&& p_t1, T2&& p_t2, Ts&&... p_ts)
        {
            p_fn(std::forward<T1>(p_t1), std::forward<T2>(p_t2));

            if constexpr (sizeof...(p_ts) != 0)
            {
                (*this)(p_fn, std::forward<Ts>(p_ts)...);
            }
        }
    };

    /// <summary>
    /// Iteration specialization for stride size of 3.
    /// </summary>
    template <>
    struct iter_n<3>
    {
        template <typename Fn, typename T1, typename T2, typename T3, typename... Ts>
        void operator()(Fn&& p_fn, T1&& p_t1, T2&& p_t2, T3&& p_t3, Ts&&... p_ts)
        {
            p_fn(std::forward<T1>(p_t1), std::forward<T2>(p_t2), std::forward<T3>(p_t3));

            if constexpr (sizeof...(p_ts) != 0)
            {
                (*this)(p_fn, std::forward<Ts>(p_ts)...);
            }
        }
    };

    /// <summary>
    /// Helper struct for checking if a specific stride size is supported. This is the case where the
    /// specialization does not match.
    /// </summary>
    /// <typeparam name="Stride">The stride size to check.</typeparam>
    /// <typeparam>Placeholder type for checking if unspecialized type alias exists on the iter_n
    /// instantiation.</typeparam>
    template <std::size_t Stride, typename = void>
    struct iter_stride_specialized : std::true_type {};

    /// <summary>
    /// Helper struct for checking if a specific stride size is supported. This is the case where the
    /// specialization does match.
    /// </summary>
    /// <typeparam name="Stride">The stride size to check.</typeparam>
    template <std::size_t Stride>
    struct iter_stride_specialized<Stride, std::void_t<typename iter_n<Stride>::unspecialized>> : std::false_type {};
}
