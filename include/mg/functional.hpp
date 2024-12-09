#pragma once

#include "detail/function_traits.hpp"
#include "detail/iter_n.hpp"
#include "sequence.hpp"
#include "types.hpp"

#include <cstdlib>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace mg
{
    /// <summary>
    /// A perfectly forwarded functor wrapper around a callable which can be stored in a template.
    /// Supported examples would include function pointers, references, capture-less lambdas,
    /// constexpr functor objects, member function pointers, member pointers, etc. The calling
    /// semantics are the same as those of std::invoke, which means that wrapped member (function) 
    /// pointers are more flexible than would otherwise appear by raw syntax.
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
            return std::invoke(Fn, std::forward<Ts>(p_ts)...);
        }
    };

    /// <summary>
    /// A perfectly forwarded functor wrapper which handles functions with default arguments. In
    /// general, these need to be provided when calling a function via an opaque handle which can
    /// be done in the same framework by having providers which return the value to use. The
    /// values returned by these providers are appended to the parameter set passed into the
    /// callable operator, and should function as one would expect.
    ///
    /// Since default parameters in general are static, unchanging, and not dynamic (i.e. it is
    /// static code), the providers are required to be default constructible and also will be called
    /// for every invocation (just like normal default parameters).
    /// </summary>
    /// <typeparam name="Fn">The callable type that is being wrapped.</typeparam>
    /// <typeparam name="...DefaultProviders">The types of the providers.</typeparam>
    template <auto Fn, typename... DefaultProviders>
    struct func_with_defaults
    {
        /// <summary>
        /// Create the callable, default constructing all the providers.
        /// </summary>
        func_with_defaults()
            : m_providers(DefaultProviders()...)
        {}

        template <typename... Ts>
        decltype(auto)
            operator()(Ts&&... p_ts)
            const
            noexcept(mg::is_nothrow_applyable_v<
                decltype(Fn),
                typename mg::concat<
                    mg::type_sequence<Ts&&...>,
                    mg::slice<
                        mg::type_sequence<std::invoke_result_t<DefaultProviders>...>,
                        default_offset(sizeof...(Ts)),
                        std::tuple_size_v<std::tuple<DefaultProviders...>>>>::as_tuple>)
        {
            auto impl = [&]<std::size_t... Is, typename... Inners>(std::index_sequence<Is...>, Inners&&... p_inners)
            {
                return std::invoke(Fn, std::forward<Inners>(p_inners)..., std::get<Is>(m_providers)()...);
            };

            return impl(
                make_index_offset_sequence<
                    default_offset(sizeof...(p_ts)),
                    std::tuple_size_v<decltype(m_providers)>>{},
                std::forward<Ts>(p_ts)...);
        }

    private:
        static constexpr std::size_t default_offset(std::size_t p_paramCount)
        {
            auto minimum_param_count = detail::function_traits<decltype(Fn)>::calling_arity - sizeof...(DefaultProviders);
            auto default_offset = p_paramCount - minimum_param_count;

            return default_offset;
        }

        /// <summary>
        /// The objects which will provide default parameter values.
        /// </summary>
        std::tuple<DefaultProviders...> m_providers;
    };

    /// <summary>
    /// A functor that always provides a template injected constant. The functor is defined so that
    /// it can be called with any arguments and will always provide the same constant.
    /// </summary>
    /// <typeparam name="C">The template injected constant.</typeparam>
    template <auto C>
    struct constant_func
    {
        /// <summary>
        /// Return the injected constant. Independent of any specific signature.
        /// </summary>
        /// <typeparam name="...Ts">The types of the arguments.</typeparam>
        /// <param name="...p_ts">The arguments that were passed to the function (but are not considered in anyway).</param>
        /// <returns>The injected constant by value.</returns>
        template <typename... Ts>
        decltype(C) operator()(Ts&&... p_ts) const noexcept
        {
            return C;
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