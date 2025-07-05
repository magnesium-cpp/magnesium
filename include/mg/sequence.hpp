#pragma once

#include "detail/sequence.hpp"

#include <cstdlib>

namespace mg
{
    template <typename T, T Min, T Max, T Stride = 1>
    using make_offset_sequence = decltype(detail::make_offset_sequence<T, Min, Max, Stride>());

    template <std::size_t Min, std::size_t Max, std::size_t Stride = 1>
    using make_index_offset_sequence = decltype(detail::make_offset_sequence<std::size_t, Min, Max, Stride>());

    template <typename... Ts>
    struct type_sequence
    {
        static constexpr std::size_t size = sizeof...(Ts);

        using as_tuple = std::tuple<Ts...>;

        template <std::size_t Idx>
        using nth = std::tuple_element_t<Idx, as_tuple>;

    };

    template <typename... T1, typename... T2>
    type_sequence<T1..., T2...> concat_impl(type_sequence<T1...>, type_sequence<T2...>)
    {
        return {};
    }

    template <std::size_t Start, std::size_t Left, typename... Ts>
    auto slice_impl(type_sequence<Ts...> seq)
    {
		auto pop_front = []<typename InnerT, typename... InnerTs>(type_sequence<InnerT, InnerTs...>)
		{
			return type_sequence<InnerTs...>{};
		};

        if constexpr (sizeof...(Ts) <= Start || Left == 0)
        {
            return type_sequence<>{};
        }
        else if constexpr (Start > 0)
        {
            auto pop_front = []<typename InnerT, typename... InnerTs>(type_sequence<InnerT, InnerTs...>)
            {
                return type_sequence<InnerTs...>{};
            };
            auto tail = pop_front(seq);
            return slice_impl<Start - 1, Left - 1>(tail);
        }
        else
        {
            auto get_head = []<typename InnerT, typename... InnerTs>(type_sequence<InnerT, InnerTs...>)
            {
                return type_sequence<InnerT>{};
            };
            auto head = get_head(seq);
            auto tail = pop_front(seq);

            return concat_impl(head, slice_impl<0, Left - 1>(tail));
        }
    }

    template <typename Seq1, typename Seq2>
    using concat = decltype(concat_impl(Seq1{}, Seq2{}));

    template <typename Seq, std::size_t Min, std::size_t Max>
    using slice = decltype(slice_impl<Min, Max>(Seq{}));
}