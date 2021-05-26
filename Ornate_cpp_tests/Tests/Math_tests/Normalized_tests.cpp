#include "pch.h"
#include "CppUnitTest.h"

#include <Math/Normalized.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Normalized_tests)
{
public:
	template<class T>
	void test_for_type()
	{
		mth::Normalized<T> zero(0);

		mth::Normalized<T> half = zero + 0.5;
		Assert::IsTrue(mth::are_similar(half.as_floating(), 0.5,0.01));

		mth::Normalized<T> quarter = half / 2;
		Assert::IsTrue(mth::are_similar(quarter.as_floating(), 0.25, 0.01));

		mth::Normalized<T> unit = quarter * 4;
		Assert::IsTrue(mth::are_similar(unit.as_floating(), 1, 0.04));

		mth::Normalized<T> half2 = unit - 0.5f;
		Assert::IsTrue(mth::are_similar(half2.as_floating(), 0.5, 0.04));
	}

	TEST_METHOD(normalized_test_uint8)
	{
		test_for_type<uint8_t>();
	}
	TEST_METHOD(normalized_test_uint16)
	{
		test_for_type<uint16_t>();
	}
	TEST_METHOD(normalized_test_uint32)
	{
		test_for_type<uint32_t>();
	}

	TEST_METHOD(normalized_test_int8)
	{
		test_for_type<int8_t>();
	}
	TEST_METHOD(normalized_test_int16)
	{
		test_for_type<int16_t>();
	}
	TEST_METHOD(normalized_test_int32)
	{
		test_for_type<int32_t>();
	}
};