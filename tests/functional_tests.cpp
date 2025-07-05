#include <gtest/gtest.h>

#include <mg/functional.hpp>
#include <mg/detail/function_traits.hpp>

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

TEST(func, mem_fn) {
    struct Integer
    {
        int Increment()
        {
            return ++val;
        }

        int Add(int x)
        {
            return (val += x);
        }

        int Decrement(int step = 1)
        {
            return (val -= step);
        }

        int val = 0;
    };

    Integer obj{};
    auto incrementMemFn = mg::func<&Integer::Increment>();
    ASSERT_EQ(incrementMemFn(obj), 1);
    ASSERT_EQ(incrementMemFn(&obj), 2);

    auto addMemFn = mg::func<&Integer::Add>();
    ASSERT_EQ(addMemFn(obj, 2), 4);

    auto decrementMemFn = mg::func_with_defaults<&Integer::Decrement, mg::constant_func<1>>();
    ASSERT_EQ(decrementMemFn(obj), 3);
    ASSERT_EQ(decrementMemFn(obj, 2), 1);
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

