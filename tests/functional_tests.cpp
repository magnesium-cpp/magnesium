#include <gtest/gtest.h>

#include <mg/functional.hpp>

#include <string>

std::string concat(std::string pre, std::string post)
{
	return pre + post;
}

TEST(func, basic) {
    static auto capturelessLambda = [] { return std::string("test_lambda"); };
    auto meaninglessFuncWrapper = mg::func<capturelessLambda>{};
    EXPECT_EQ(meaninglessFuncWrapper(), std::string("test_lambda"));

    EXPECT_EQ(mg::func<concat>{}("hello ", "world"), std::string("hello world"));
}

TEST(constant_func, fundamental_types) {
    auto f2 = mg::constant_func<2>();
    EXPECT_EQ(f2(), 2);
    EXPECT_EQ(f2(1, 5, 3.4, ""), 2);

    static uint8_t counter = 0;
    auto getCounterPtr = mg::constant_func<&counter>();
    EXPECT_EQ(++(*getCounterPtr()), 1);
    EXPECT_EQ(++(*getCounterPtr()), 2);
    EXPECT_EQ(++(*getCounterPtr()), 3);
}

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

