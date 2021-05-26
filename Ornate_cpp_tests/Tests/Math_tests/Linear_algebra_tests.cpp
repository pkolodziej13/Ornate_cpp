#include "pch.h"
#include "CppUnitTest.h"

#include <Math/Used_linear_algebra.h>
#include <Math/Direction_3d.h>
#include <Math/Geometry_utilities_3d.h>

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
	TEST_METHOD(cross_perpedicular_normalized)
	{
		mth::vec3 v1{ 1,0,0 };
		mth::vec3 v2{ 0,1,0 };
		mth::vec3 result_vector = mth::cross(v1,v2);

		Assert::IsTrue(mth::are_similar(result_vector.length(), 1.f));
	}
	TEST_METHOD(cross_perpedicular_magnified)
	{
		mth::vec3 v1{ 2,0,0 };
		mth::vec3 v2{ 0,1,0 };
		mth::vec3 result_vector = mth::cross(v1, v2);

		Assert::IsTrue(mth::are_similar(result_vector.length(), 2.f));
	}
	TEST_METHOD(cross_sharpangled_magnified)
	{
		mth::vec3 v1{ 1.23f,1.34f,1.45f };
		mth::vec3 v2{ 0,1,0 };
		mth::vec3 result_vector = mth::cross(v1, v2);

		float expected_length = (v1 - v2 * v1.dot( v2)).length();

		Assert::IsTrue(mth::are_similar(result_vector.length(), expected_length));
	}
	TEST_METHOD(cross_obtuse_magnified)
	{
		mth::vec3 v1{ 1.23f,-1.34f,1.45f };
		mth::vec3 v2{ 0,1,0 };
		mth::vec3 result_vector = mth::cross(v1, v2);

		float expected_length = (v1 - v2 * v1.dot(v2)).length();

		Assert::IsTrue(mth::are_similar(result_vector.length(), expected_length));
	}
	TEST_METHOD(cross_perpedicular_nonunit)
	{
		mth::vec3 v1{ 2.f,0,  0 };
		mth::vec3 v2{ 0,  2.f,0 };
		mth::vec3 result_vector = mth::cross(v1, v2);

		float result = result_vector.length();

		Assert::IsTrue(mth::are_similar(result, 4.f));
	}
	TEST_METHOD(cross_nonunit)
	{
		mth::vec3 v1{ 1.23f,-1.34f,7.45f };
		mth::vec3 v2{ 4.32f,6.66f,0.87f };
		mth::vec3 result_vector = mth::cross(v1, v2);

		float expected_length = mth::geo3::triangle_field(mth::vec3(0,0,0), v1, v2)*2;
		float result = result_vector.length();

		Assert::IsTrue(mth::are_similar(result, expected_length));
	}

};