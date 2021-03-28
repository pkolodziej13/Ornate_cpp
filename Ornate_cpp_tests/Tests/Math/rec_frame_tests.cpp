#include "pch.h"
#include "CppUnitTest.h"

#include <Math/rec_frame.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(rec_frame_tests)
{
public:
	TEST_METHOD(from_unit_test)
	{
		mth::rec_frame<float> rec1{ mth::vec2{0.5f},mth::vec2{0.5f} };
		auto rec2 = rec1.from_unit<0>({ mth::vec2{0.25f},mth::vec2{0.5f} });

		Assert::IsTrue(mth::linear_algebra::are_similar(rec2.min, mth::vec2(0.625f,0.625)));
		Assert::IsTrue(mth::linear_algebra::are_similar(rec2.max, mth::vec2(0.875f, 0.875)));
	}
};