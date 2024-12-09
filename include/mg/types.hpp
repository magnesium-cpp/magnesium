#pragma once

#include "detail/is_instance.hpp"

#include <tuple>
#include <type_traits>

namespace mg
{
    /// <summary>
    /// Check if a type is an instantiation of a template. The check is independent of reference, const, and volatile
    /// modifiers.
    /// </summary>
    /// <typeparam name="T">The type to check as an instantiation of a template.</typeparam>
    /// <template name="U">The template to check against.</template>
    template <typename T, template <typename...> typename U>
    using is_instance_t = detail::is_instance<std::remove_cvref_t<T>, U>;

    /// <summary>
    /// The value alias for the template instantiation check.
    /// </summary>
    /// <typeparam name="T">The type to check as an instantiation of a template.</typeparam>
    /// <template name="U">The template to check against.</template>
    template <typename T, template <typename ...> typename U>
    constexpr bool is_instance_v = is_instance_t<T, U>::value;

    template <typename Fn, typename Tuple>
    using is_applyable = std::is_invocable<decltype(std::apply<Fn, Tuple>), Fn, Tuple>;

    template <typename Fn, typename Tuple>
    constexpr bool is_applyable_v = is_applyable<Fn, Tuple>::value;

    template <typename Fn, typename Tuple>
    using is_nothrow_applyable = std::is_nothrow_invocable<decltype(std::apply<Fn, Tuple>), Fn, Tuple>;

    template <typename Fn, typename Tuple>
    constexpr bool is_nothrow_applyable_v = is_nothrow_applyable<Fn, Tuple>::value;

    template <typename R, typename Fn, typename Tuple>
    using is_nothrow_applyable_r = std::is_nothrow_invocable_r<R, decltype(std::apply<Fn, Tuple>), Fn, Tuple>;

    template <typename R, typename Fn, typename Tuple>
    constexpr bool is_nothrow_applyable_r_v = is_nothrow_applyable_r<R, Fn, Tuple>::value;
}