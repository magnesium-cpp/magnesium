#pragma once

#include <cstdlib>
#include <tuple>
#include <type_traits>
#include <utility>

namespace mg::detail
{
    /// <summary>
    /// Implementation (helper) for mg::iter_zipped_tuples. Confer with documentation on that
    /// function for details on usage.
    /// </summary>
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

    /// <summary>
    /// Implementation (helper) for mg::tuple_map. Confer with documentation on that function for
    /// details on usage.
    /// </summary>
    template <typename Fn, typename Tuple, std::size_t... I>
    auto tuple_map_impl(Tuple&& p_tuple, Fn&& p_fn, std::index_sequence<I...>)
    {
        return std::make_tuple(p_fn(std::get<I>(std::forward<Tuple>(p_tuple)))...);
    }
}