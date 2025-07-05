#pragma once

#include "mg/functional.hpp"
#include "mg/utility.hpp"

#include <cstdlib>
#include <tuple>
#include <type_traits>
#include <unordered_set>
#include <utility>

#pragma region passthroughs

/// <summary>
/// All of the functions within this namespace block are simple implementation passthroughs
/// of the public methods in the corresponding files. For complete documentation on a given
/// method see this file.
/// </summary>
namespace mg::detail
{
    template <std::size_t Idx, std::size_t Max, typename TFn, typename... Tuples>
    bool iter_zipped_tuples_impl(TFn&& p_fn, Tuples&&... p_tuples)
    {
        if constexpr (Max == 0)
        {
            return true;
        }
        else
        {
            if constexpr (std::is_assignable_v<bool&, std::invoke_result_t<TFn&&, decltype(std::get<Idx>(std::forward<Tuples>(p_tuples)))...>>)
            {
                bool continueIteration = std::invoke(p_fn, std::get<Idx>(std::forward<Tuples>(p_tuples))...);
                if (!continueIteration)
                {
                    return false;
                }
            }
            else
            {
                std::invoke(p_fn, std::get<Idx>(std::forward<Tuples>(p_tuples))...);
            }

            if constexpr ((Idx + 1) < Max)
            {
                return iter_zipped_tuples_impl<Idx + 1, Max>(std::forward<TFn>(p_fn), std::forward<Tuples>(p_tuples)...);
            }
            else
            {
                return true;
            }
        }
    }

    template <typename Fn, typename Tuple, std::size_t... I>
    auto tuple_map_impl(Tuple&& p_tuple, Fn&& p_fn, std::index_sequence<I...>)
    {
        return std::make_tuple(p_fn(std::get<I>(std::forward<Tuple>(p_tuple)))...);
    }

    template<
        typename Iter,
        typename Hash,
        typename Equal,
        typename Alloc>
    bool all_unique_impl(Iter first, Iter last, const Hash& hash, const Equal& equal, const Alloc& alloc)
    {
        using Ptr = std::add_pointer_t<std::remove_reference_t<std::iter_reference_t<Iter>>>;
        using T = std::remove_cvref_t<std::iter_reference_t<Iter>>;
        using ReboundAlloc = std::allocator_traits<Alloc>::template rebind_alloc<Ptr>;

        std::unordered_set<
            Ptr,
            mg::map_args<Hash, mg::deref>,
            mg::map_args<Equal, mg::deref>,
            ReboundAlloc> identity(0, hash, equal, alloc);

        while (first != last) {
            auto [_, inserted] = identity.emplace(std::addressof(*first));
            if (!inserted) {
                return false;
            }

            ++first;
        }

        return true;
    }
}

#pragma endregion