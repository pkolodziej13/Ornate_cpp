#include "pch.h"
#include "CppUnitTest.h"

#include <Math/Model_matrix.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Model_matrix_tests)
{
public:
	TEST_METHOD(trs_set_get_test)
	{
		mth::Model_matrix<float, 2>  trs;
		mth::vec2 v1{ 1,0 };
		trs.translate(v1);
		auto res = trs.translation();
		Assert::IsTrue(mth::linear_algebra::are_similar(v1, trs.translation()));

	}
	TEST_METHOD(trs_move_rotate_get_test)
	{
		mth::Model_matrix<float, 2>  trs;
		mth::vec2 v1{ 1,0 };
		trs.translate(v1);
		trs.rotate(mth::Turn(0.25));
		trs.translate(v1);

		auto ioi = trs.rotation();
		auto oo = trs.rotation().turn();

		Assert::IsTrue(mth::linear_algebra::are_similar(mth::vec2{ 1,-1 }, trs.translation()));
		Assert::IsTrue(mth::are_similar(mth::Turn(0.25), trs.rotation().turn()));

	}
	TEST_METHOD(trs_rot_rot_test)
	{
		mth::Model_matrix<float, 2>  trs;
		trs.rotate(mth::Turn(0.25f));
		trs.rotate(mth::Turn(0.1f));

		Assert::IsTrue(are_similar(mth::Turn(0.35f), mth::geo2::Rotation_matrix(trs.rotation()).turn()));
	}
	TEST_METHOD(trs_tra_rot_sca_tra_rot_sca_test)
	{
		mth::Model_matrix<float, 2>  trs;
		mth::vec2 v1{ 1,0 };
		trs.translate(v1);
		trs.rotate(mth::Turn(0.25));
		trs.scale(2);

		Assert::IsTrue(mth::linear_algebra::are_similar(mth::vec2{ 1,0 }, trs.translation()));
		Assert::IsTrue(mth::are_similar(mth::Turn(0.25), mth::geo2::Rotation_matrix(trs.rotation()).turn()));
		Assert::IsTrue(mth::linear_algebra::are_similar(mth::vec2{ 2 }, trs.scale()));

		trs.translate(v1);
		trs.rotate(mth::Turn(0.25));
		trs.scale(2);

		Assert::IsTrue(mth::linear_algebra::are_similar(mth::vec2{ 1,-2 }, trs.translation()));
		Assert::IsTrue(mth::are_similar(mth::Turn(0.5), mth::geo2::Rotation_matrix(trs.rotation()).turn()));
		Assert::IsTrue(mth::linear_algebra::are_similar(mth::vec2{ 4 }, trs.scale()));
	}


};