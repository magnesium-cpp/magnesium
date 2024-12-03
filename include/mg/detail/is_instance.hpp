#pragma once

#include <type_traits>

namespace mg::detail
{
    /// <summary>
    /// False case on implementation when checking if a class is an instantiation of a template.
    /// </summary>
    /// <typeparam>The type that is being checked for.</typeparam>
    /// <template>The template that is being checked against.</template>
    template <typename, template <typename...> typename>
    struct is_instance : public std::false_type {};

    /// <summary>
    /// True case on implementation when checking if a class is an instantiation of a template.
    /// </summary>
    /// <template name="U">The template that is being checked against.</template>
    /// <typeparam name="...Ts">The types that instantiate the template that is being checked against.</typeparam>
    template <template <typename...> typename U, typename... Ts>
    struct is_instance<U<Ts...>, U> : public std::true_type {};
}
