#include <gtest/gtest.h>

#include <mg/functional.hpp>

TEST(iter_n_test, stride_1) {
    int sum = 0;
    mg::iter_n<1>([&](int el) { sum += el;  }, 1, 2, 3, 4);
    EXPECT_EQ(sum, 10);
}

TEST(iter_n_test, stride_2) {
    int sum = 0;
    mg::iter_n<2>(
        [&](int first, int second) { sum += (first * second); },
        1, 2, 3, 4);
    EXPECT_EQ(sum, 14);
}

TEST(iter_n_test, stride_3) {
    int sum = 0;
    mg::iter_n<3>(
        [&](int first, int second, int third) { sum += (first * second - third); },
        1, 2, 3, 4, 5, 6);
    EXPECT_EQ(sum, 13);
}

