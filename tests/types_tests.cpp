#include <gtest/gtest.h>

#include <mg/types.hpp>

#include <tuple>

template <typename T>
struct SingleTemplate {};

template <typename T1, typename T2>
struct MultiTemplate {};

TEST(is_instance, basic_single) {
    EXPECT_TRUE((mg::is_instance_v<SingleTemplate<int>, SingleTemplate>));
    EXPECT_TRUE((mg::is_instance_v<SingleTemplate<int>&&, SingleTemplate>));
    EXPECT_TRUE((mg::is_instance_v<const SingleTemplate<int>&, SingleTemplate>));
    EXPECT_TRUE((mg::is_instance_v<volatile SingleTemplate<int>&, SingleTemplate>));

    EXPECT_FALSE((mg::is_instance_v<int, SingleTemplate>));
    EXPECT_FALSE((mg::is_instance_v<const int&, SingleTemplate>));
}

TEST(is_instance, multi) {
    EXPECT_TRUE((mg::is_instance_v<MultiTemplate<int, float>, MultiTemplate>));
    EXPECT_FALSE((mg::is_instance_v<std::tuple<int, float>, MultiTemplate>));
}

TEST(is_applyable, basic) {
    struct Functor
    {
        void operator()(int, double, int) {}
    };

    EXPECT_TRUE((mg::is_applyable_v<Functor, std::tuple<int, double, int>>));
    EXPECT_TRUE((mg::is_applyable_v<Functor, std::tuple<const int&, double&&, int>>));
}
