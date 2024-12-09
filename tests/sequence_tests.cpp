#include <gtest/gtest.h>

#include <type_traits>

#include <mg/sequence.hpp>

TEST(integer_sequence, basic_assertions) {
    EXPECT_TRUE((std::is_same_v<
        mg::make_offset_sequence<int, 1, 3>,
        std::integer_sequence<int, 1, 2>>));

    EXPECT_TRUE((std::is_same_v<
        mg::make_offset_sequence<int, 4, 4>,
        std::integer_sequence<int>>));

    EXPECT_TRUE((std::is_same_v<
        mg::make_offset_sequence<int, 4, 3>,
        std::integer_sequence<int>>));

    EXPECT_TRUE((std::is_same_v<
        mg::make_offset_sequence<int, -1, 5>,
        std::integer_sequence<int, -1, 0, 1, 2, 3, 4>>));
}

TEST(integer_sequence, stride_assertions) {
    EXPECT_TRUE((std::is_same_v<
        mg::make_index_offset_sequence<4, 10, 2>,
        std::index_sequence<4, 6, 8>>));
}
