#pragma once

#include "detail/collections.hpp"

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

namespace mg
{
    /// <summary>
    /// Iterate in parallel over the elements in a defined set of tuples. Iteration will stop when the end of the
    /// smallest tuple is reached. Additionally, the callable signature may return a bool which can be used to signal
    /// whether iteration is to continue.
    /// </summary>
    /// <typeparam name="Fn">The type of the callable to invoke.</typeparam>
    /// <typeparam name="...Tuples">The set of tuples to iterate over.</typeparam>
    /// <param name="p_fn">The callable to invoke over the tuples. Will have one argument in parallel for each tuple
    /// and can signal to stop iteration by returning false.</param>
    /// <param name="...p_tuples">The tuples to iterate over.</param>
    /// <returns>False if the callable signaled iteration to stop and true otherwise.</returns>
    template <typename Fn, typename... Tuples>
    bool iter_zipped_tuples(Fn&& p_fn, Tuples&&... p_tuples)
    {
        constexpr static std::size_t DimCount = std::min({ std::tuple_size_v<std::remove_cvref_t<Tuples>>... });
        return detail::iter_zipped_tuples_impl<0, DimCount>(std::forward<Fn>(p_fn), std::forward<Tuples>(p_tuples)...);
    }

    /// <summary>
    /// Apply a callable to all members of a tuple resulting in a new tuple.
    /// </summary>
    /// <remarks>The returned value is created with the same semantics as make_tuple. This means that if one of the
    /// tuple element types should be an l-value reference, then a reference_wrapper can be returned. If the type
    /// should be an r-value reference, use another library, because I don't know how to do that or if that would be
    /// safe.</remarks>
    /// <remarks>The tuple to be mapped is taken as a universal reference which allows for individual data elements to
    /// be moved into the mapping function. It also allows for other in-place modifications but these should be avoided.</remarks>
    /// <typeparam name="Tuple">The type of the tuple being mapped.</typeparam>
    /// <typeparam name="Fn">The type of the callable being applied.</typeparam>
    /// <param name="p_tuple">The tuple being mapped.</param>
    /// <param name="p_fn">The callable being applied to each tuple element.</param>
    /// <returns>The mapped tuple.</returns>
    template <typename Fn, typename Tuple>
    decltype(auto) tuple_map(Tuple&& p_tuple, Fn&& p_fn)
    {
        return detail::tuple_map_impl(
            std::forward<Tuple>(p_tuple),
            std::forward<Fn>(p_fn),
            std::make_index_sequence<std::tuple_size_v<std::remove_cvref_t<Tuple>>>{});
    }

    /// <summary>
    /// An addressable_iterator is one which stably references underlying locations in memory whose
    /// address can be taken. A notable exception to this would be something like std::vector<bool>
    /// whose iterator does not actually reference an addressable object, but rather provides a proxy
    /// which can convert to a bool.
    ///
    /// A lot of iterators meet this concept's requirements but most things that are "smart" would not.
    /// </summary>
    template <typename It>
    concept addressable_iterator =
        std::forward_iterator<It> &&
        std::is_reference_v<std::iter_reference_t<It>>;

    /// <summary>
    /// Convenience method to check if all elements in a iterator range are unique. This is an optimized
    /// method which will not create copies of the contained elements.
    /// </summary>
    /// <typeparam name="Iter">The type of the iterator which must satisfy the addressable concept.</typeparam>
    /// <typeparam name="Hash">The type of the hasher.</typeparam>
    /// <typeparam name="Equal">The type of the equality comparator.</typeparam>
    /// <typeparam name="Alloc">The type of the allocator.</typeparam>
    /// <param name="first">The start of the range to consider, inclusive.</param>
    /// <param name="last">The end of the range, not inclusive.</param>
    /// <param name="hash">Hasher of iterator's value type.</param>
    /// <param name="equal">Equality comparator of iterator's value type.</param>
    /// <param name="alloc">The allocator to use for any memory allocation requirements of this method.</param>
    /// <returns>True if all of the elements in the iterator range do not compare unique and false otherwise.</returns>
    template<
        addressable_iterator Iter,
        typename Hash = std::hash<std::iter_value_t<Iter>>,
        typename Equal = std::equal_to<>,
        typename Alloc = std::allocator<void>>
    bool all_unique(
        Iter first,
        Iter last,
        const Hash& hash = Hash{},
        const Equal& equal = Equal{},
        const Alloc& alloc = Alloc{})
    {
        return detail::all_unique_impl(first, last, hash, equal, alloc);
    }
}