#include "pch.h"
#include "CppUnitTest.h"

#include <Math/View_matrix.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(View_matrix_tests)
{
public:
	TEST_METHOD(view_matrix_default)
	{
		mth::geo3::View_matrix<float> vm(mth::vec3(), mth::geo3::Front_up{});

		auto fu = vm.rotation().front_up();
		auto e = vm.eye();

		Assert::IsTrue(mth::linear_algebra::are_similar(fu.front().vec(), mth::geo3::front.vec()));
		Assert::IsTrue(mth::linear_algebra::are_similar(fu.up().vec(), mth::geo3::up.vec()));
		Assert::IsTrue(mth::linear_algebra::are_similar(e, mth::vec3()));
	}
	TEST_METHOD(view_matrix_eye_position_test)
	{
		mth::geo3::Front_up direction_o{ mth::vec3(0.5f,-0.6f,0.1f) };
		mth::vec3 eye_o{ 1,-2,3 };

		mth::geo3::View_matrix<float> vm(eye_o, direction_o);

		auto eye_c = vm.eye();

		Assert::IsTrue(mth::linear_algebra::are_similar(eye_o, eye_c));
	}
	TEST_METHOD(view_matrix_werid)
	{
		mth::geo3::Front_up direction_o{ mth::vec3(0.5f,-0.6f,0.1f) };
		mth::vec3 eye_o{ 1,-2,3 };

		mth::geo3::View_matrix<float> vm(eye_o, direction_o);

		auto direction_c = vm.rotation().front_up();
		auto eye_c = vm.eye();

		Assert::IsTrue(mth::linear_algebra::are_similar(direction_o.front().vec(), direction_c.front().vec()));
		Assert::IsTrue(mth::linear_algebra::are_similar(direction_o.up().vec(), direction_c.up().vec()));
		Assert::IsTrue(mth::linear_algebra::are_similar(eye_o, eye_c));
	}
};