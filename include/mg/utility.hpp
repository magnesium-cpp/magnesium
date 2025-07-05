#pragma once

#include <cstdlib>
#include <utility>

namespace mg
{
    struct deref
    {
        template <typename Ptr>
        decltype(auto) operator()(Ptr&& p_ptr) const
        {
            return *std::forward<Ptr>(p_ptr);
        }
    };

    template <std::size_t Idx>
    struct get
    {
        template <typename T>
        decltype(auto) operator()(T&& p_arg) const
        {
            if constexpr (requires { std::get<Idx>(std::forward<T>(p_arg)); })
            {
                return std::get<Idx>(std::forward<T>(p_arg));
            }
            else if constexpr (requires { std::forward<T>(p_arg)[Idx]; })
            {
                return std::forward<T>(p_arg)[Idx];
            }
            else
            {
                static_assert(false, "The type provided cannot be indexed.");
            }
        }
    };

}