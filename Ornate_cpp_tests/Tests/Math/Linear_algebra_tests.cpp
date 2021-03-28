#include "pch.h"
#include "CppUnitTest.h"

#include <Math/Used_linear_algebra.h>
#include <Math/Direction_3d.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Linear_algebra_tests)
{
public:
	TEST_METHOD(vectors_test)
	{
		mth::g_vec<double, 2> v1;
		mth::g_vec<float, 2> v2;

		auto v3 = v1 + v2 * 0.7f;

		Assert::IsTrue(mth::linear_algebra::are_similar(mth::g_vec<double, 2>(0), v3));
	}
	
	TEST_METHOD(mat2_inverting_test)
	{
		mth::g_mat<float, 2, 2> mat1{ {1,2},{3,4} };
		auto mat2 = mat1.inversed().inversed();

		Assert::IsTrue(are_similar(mat1, mat2));
	}
	TEST_METHOD(mat3_inverting_test)
	{
		mth::g_mat<float, 3, 3> mat1{ {1,2,3},{4,5,6},{7,8,9} };
		auto mat2 = mat1.inversed().inversed();

		Assert::IsTrue(are_similar(mat1, mat2));
	}
	TEST_METHOD(mat4_inverting_test)
	{
		mth::g_mat<float, 4, 4> mat1{ {1,2,3,4},{4,5,6,7},{7,8,9,10},{1,3,4,11} };
		auto mat2 = mat1.inversed().inversed();

		Assert::IsTrue(are_similar(mat1, mat2));
	}

};