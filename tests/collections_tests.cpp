#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <mg/collections.hpp>

#include <array>
#include <functional>
#include <list>
#include <span>
#include <string>
#include <tuple>
#include <vector>

TEST(iter_zipped_tuples, basic) {
    auto ones = std::make_tuple(1, 2, 3);
    auto twos = std::make_tuple(2, 4, 6);
    auto threes = std::make_tuple(3, 6, 9);

    mg::iter_zipped_tuples(
        [](auto one, auto two, auto three)
        {
            EXPECT_EQ(one, two / 2);
            EXPECT_EQ(two / 2, three / 3);
        },
        ones, twos, threes);
}

TEST(iter_zipped_tuples, uneven) {
    auto longer = std::make_tuple(1, 2, 3);
    auto shorter = std::make_tuple(4, 5);

    std::vector<int> nums;

    mg::iter_zipped_tuples(
        [&](auto i, auto j)
        {
            nums.push_back(i);
            nums.push_back(j);
        },
        longer, shorter);

    ASSERT_THAT(nums, testing::ElementsAre(1, 4, 2, 5));
}

TEST(iter_zipped_tuples, uneven_zero_length) {
    auto longer = std::make_tuple(1, 2, 3);
    auto shorter = std::make_tuple(4, 5);
    auto zero = std::make_tuple();

    std::vector<int> nums;

    mg::iter_zipped_tuples(
        [&](auto i, auto j, auto k)
        {
            nums.push_back(i);
            nums.push_back(j);
            nums.push_back(k);
        },
        longer, shorter, zero);

    ASSERT_THAT(nums, testing::ElementsAre());
}

TEST(tuple_map, from_values) {
    auto mapped = mg::tuple_map(
        std::tuple{ 1, 10, 100, 1000 },
        [](int el) { return el * 10; });
    EXPECT_EQ(std::get<0>(mapped), 10);
    EXPECT_EQ(std::get<1>(mapped), 100);
    EXPECT_EQ(std::get<2>(mapped), 1000);
    EXPECT_EQ(std::get<3>(mapped), 10000);
}

TEST(tuple_map, from_references) {
    struct IntPair
    {
        int val;
        int secondary;
    };

    auto a = IntPair{ 0, 1 };
    auto b = IntPair{ 10, 11 };

    auto mapped = mg::tuple_map(
        std::make_tuple(std::ref(a), std::ref(b)),
        [](auto& el) {return std::ref(el.secondary); });

    EXPECT_EQ(&std::get<0>(mapped), &a.secondary);
    EXPECT_EQ(&std::get<1>(mapped), &b.secondary);
}

TEST(tuple_map, moved_into) {
    struct Helper
    {
        bool m_moveConstructedInto = false;

        Helper() = default;
        Helper(const Helper&) = default;
        Helper& operator=(const Helper&) = default;
        Helper& operator=(Helper&&) = default;

        Helper(Helper&&) noexcept
            : m_moveConstructedInto(true) {}
    };

    auto h = Helper{};
    auto ch = Helper{};

    auto moved_into = mg::tuple_map(
        std::make_tuple(std::ref(h), std::cref(ch), Helper{}, Helper{}),
        [](Helper helper) { return helper.m_moveConstructedInto; });

    EXPECT_EQ(moved_into, std::make_tuple(false, false, true, true));
}

TEST(all_unique, empty) {
    std::array<int, 0> arr{};
    EXPECT_TRUE(mg::all_unique(std::begin(arr), std::end(arr)));
}

TEST(all_unique, single) {
    std::array arr{1};
    EXPECT_TRUE(mg::all_unique(std::begin(arr), std::end(arr)));
}

TEST(all_unique, duplicates) {
    std::array arr1{ 0, 1, 2, 3, 4, 0 };
    EXPECT_FALSE(mg::all_unique(std::begin(arr1), std::end(arr1)));

    std::array arr2{ 0, 1, 2, 3, 4, 4 };
    EXPECT_FALSE(mg::all_unique(std::begin(arr2), std::end(arr2)));

    std::vector vec1{ 1, 2, 3, -1, -2, -3, 3 };
    EXPECT_FALSE(mg::all_unique(std::begin(vec1), std::end(vec1)));

    std::list l{ 0, 1, 0 };
    EXPECT_FALSE(mg::all_unique(std::begin(l), std::end(l)));

    int cArr[]{ 0, 1, 2, 1 };
    EXPECT_FALSE(mg::all_unique(std::begin(cArr), std::end(cArr)));
}

template <typename T>
concept has_all_unique = requires(T& a) {
	mg::all_unique(std::begin(a), std::end(a));
};

TEST(all_unique, valid_containers) {
    static_assert(!has_all_unique<std::vector<bool>>);
    static_assert(has_all_unique<std::vector<int>>);
    static_assert(has_all_unique<std::string>);
    static_assert(has_all_unique<int[3]>);
    static_assert(has_all_unique<std::span<float>>);
}
