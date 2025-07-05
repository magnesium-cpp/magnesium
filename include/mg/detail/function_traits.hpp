#pragma once

#include <cstdlib>
#include <tuple>

/// <summary>
/// Implementation of function_traits for internal usage at the moment. Full implementation is quite
/// complicated and extensive, and not worth creating at the moment for the desired existing use
/// cases.
/// </summary>

namespace mg::detail
{
    /// <summary>
    /// Implementation logic for function_traits. The specializations will provide the necessary
    /// template information which will be surfaced as necessary. In general, the function_traits
    /// object will decompose the given type as necessary and this impl will simply expose the
    /// information uniformly to clients.
    /// </summary>
    /// <typeparam name="MemFn">Flag if the function is a member function pointer.</typeparam>
    /// <typeparam name="R">The return type.</typeparam>
    /// <typeparam name="...Params">The required arguments types (does not include this for member
    /// function).</typeparam>
    template <bool MemFn, bool NoExcept, typename R, typename... Params>
    struct function_traits_impl
    {
        /// <summary>
        /// The type of object that is returned by the function.
        /// </summary>
        using return_type = R;

        /// <summary>
        /// A type pack of all the parameter types of this function.
        /// </summary>
        using params = std::tuple<Params...>;

        /// <summary>
        /// Get the type of the Nth parameter.
        /// </summary>
        /// <typeparam name="N">The index of the parameter.</typeparam>
        template <std::size_t N>
        using param_type = std::tuple_element_t<N, params>;

        /// <summary>
        /// The number of arguments that the function takes. This is the number of parameters that
        /// are visibly present at the declaration of the function (so for member functions the
        /// implicit this is not included).
        /// </summary>
        static constexpr auto arity = sizeof...(Params);

        /// <summary>
        /// The number of arguments that would need to be provided in an invoke expression with an
        /// object of the type being analyzed. Same as arity except for member function pointers
        /// where the `this` receiver would need to be provied.
        /// </summary>
        static constexpr auto calling_arity = arity + (MemFn ? 1 : 0);

        /// <summary>
        /// Flag if the function type is a member function or not.
        /// </summary>
        static constexpr bool is_member_fn = MemFn;

        /// <summary>
        /// Flag if the function type is noexcept or not.
        /// </summary>
        static constexpr bool is_noexcept = NoExcept;
    };

    /// <summary>
    /// Generalized function_traits object which provides no additional information as a fallback
    /// which means that the type provided was not understood as a callable.
    /// </summary>
    /// <typeparam name="F">The function type to extract information on.</typeparam>
    template <typename F>
    struct function_traits
    {
        static_assert(false, "No specialization option matched for the templatized function.");
    };

    /// <summary>
    /// Specialization of function traits for a free function pointer.
    /// </summary>
    /// <typeparam name="R">The return type of the function.</typeparam>
    /// <typeparam name="...Args">The argument types that the fu</typeparam>
    template <typename R, typename... Args>
    struct function_traits<R(*)(Args...)> : function_traits_impl<false, false, R, Args...> {};

    /// <summary>
    /// Specialization of function traits for a free function pointer.
    /// </summary>
    /// <typeparam name="R">The return type of the function.</typeparam>
    /// <typeparam name="...Args">The argument types that the fu</typeparam>
    template <typename R, typename... Args>
    struct function_traits<R(*)(Args...) noexcept> : function_traits_impl<false, true, R, Args...> {};

    /// <summary>
    /// Specialization of function traits for a member function pointer.
    /// </summary>
    /// <typeparam name="C"></typeparam>
    /// <typeparam name="R"></typeparam>
    /// <typeparam name="...Args"></typeparam>
    template <typename C, typename R, typename... Args>
    struct function_traits<R(C::*)(Args...)> : function_traits_impl<true, false, R, Args...> {};

    /// <summary>
    /// Specialization of function traits for a member function pointer.
    /// </summary>
    /// <typeparam name="C"></typeparam>
    /// <typeparam name="R"></typeparam>
    /// <typeparam name="...Args"></typeparam>
    template <typename C, typename R, typename... Args>
    struct function_traits<R(C::*)(Args...) noexcept> : function_traits_impl<true, true, R, Args...> {};

    /// <summary>
    /// A convenience wrapper so that decltype does not have to wrapper all the desired inputs to
    /// function_traits.
    /// </summary>
    /// <typeparam name="F">The object to get the function traits about.</typeparam>
    template <auto F>
    using function_traits_obj = function_traits<decltype(F)>;
}