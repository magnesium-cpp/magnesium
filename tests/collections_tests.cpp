#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <mg/collections.hpp>

#include <functional>
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
