#include <gtest/gtest.h>

#include <mg/math.hpp>

struct WholePowTestCase
{
	int m_base;
	int m_power;
	int m_answer;
};

class whole_pow_valid : public testing::TestWithParam<WholePowTestCase>{};

TEST_P(whole_pow_valid, assertions) {
	const auto& testCase = GetParam();

	auto actual = mg::whole_pow(testCase.m_base, testCase.m_power);
	auto expected = testCase.m_answer;
	EXPECT_EQ(actual, expected);
}

INSTANTIATE_TEST_SUITE_P(
	assertions,
	whole_pow_valid,
	testing::Values(
		WholePowTestCase{ 0, 100, 0 },
		WholePowTestCase{ 1, 1, 1 },
		WholePowTestCase{ 1, 100000, 1 },
		WholePowTestCase{ 10000, 1, 10000 },
		WholePowTestCase{ 54, 0, 1 },
		WholePowTestCase{ 2, 3, 8 },
		WholePowTestCase{ 8, 2, 64 }));

struct WholePowExceptionTestCase
{
	int m_base;
	int m_power;
};

class whole_pow_exception : public testing::TestWithParam<WholePowExceptionTestCase>{};

TEST_P(whole_pow_exception, assertions) {
	const auto& testCase = GetParam();
	EXPECT_ANY_THROW(mg::whole_pow(testCase.m_base, testCase.m_power));
}

INSTANTIATE_TEST_SUITE_P(
	assertions,
	whole_pow_exception,
	testing::Values(
		WholePowExceptionTestCase{ 4, -1 },
		WholePowExceptionTestCase{ 0, 0 },
		WholePowExceptionTestCase{ -2, 4 }));
