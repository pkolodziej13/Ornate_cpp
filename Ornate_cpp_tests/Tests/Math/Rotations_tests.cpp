#include "pch.h"
#include "CppUnitTest.h"

#include <Math/Rotation_matrix_2d.h>
#include <Math/Rotation_matrix_3d.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Rotations_tests)
{
public:
	TEST_METHOD(turn_comparasion_simple)
	{
		Assert::IsTrue(mth::are_similar(mth::Turn(1), mth::Turn(0.999999940f)));
		Assert::IsTrue(mth::are_similar(mth::Turn(0.999999940f), mth::Turn(1)));

		Assert::IsFalse(mth::are_similar(mth::Turn(0.5f), mth::Turn(1)));
	}
	TEST_METHOD(ortogonalization_simple)
	{
		mth::mat3 ma;

		mth::mat3 orto = mth::geo3::orthogonalized(ma);

		Assert::IsTrue(mth::geo3::is_unit_orthogonal(orto));
		Assert::IsTrue(are_similar(orto.transposed(), orto.inversed()));

		mth::geo3::Direction orginal_sum{ ma.x + ma.y + ma.z };
		mth::geo3::Direction processed_sum{ orto.x + orto.y + orto.z };
		Assert::IsTrue(mth::are_similar(std::abs(orginal_sum.dot(processed_sum)), 1));


		Assert::IsTrue(mth::geo3::Direction(orginal_sum.perpendicular(ma.x)).dot(orginal_sum.perpendicular(orto.x)) > 0);
		Assert::IsTrue(mth::geo3::Direction(orginal_sum.perpendicular(ma.y)).dot(orginal_sum.perpendicular(orto.y)) > 0);
		Assert::IsTrue(mth::geo3::Direction(orginal_sum.perpendicular(ma.z)).dot(orginal_sum.perpendicular(orto.z)) > 0);

	}
	TEST_METHOD(ortogonalization_werid)
	{

		mth::vec3 rihgt{ 1,0,0 };
		mth::vec3 up{ 0,1,0 };
		mth::vec3 front{ 0,0,1 };

		mth::vec3 m_rihgt = -up * 0.7f + rihgt * 0.3f;
		mth::vec3 m_up = -up * 0.7f - rihgt * 0.7f + front * 0.3f;
		mth::vec3 m_front = up * 0.7f + rihgt * 0.3f + front * 0.5f;

		mth::mat3 ma{ m_rihgt ,m_up ,m_front };

		mth::mat3 orto = mth::geo3::orthogonalized(ma);

		Assert::IsTrue(mth::geo3::is_unit_orthogonal(orto));
		Assert::IsTrue(are_similar(orto.transposed(), orto.inversed()));


		mth::geo3::Direction orginal_sum{ ma.x + ma.y + ma.z };
		mth::geo3::Direction processed_sum{ orto.x + orto.y + orto.z };
		Assert::IsTrue(mth::are_similar(std::abs(orginal_sum.dot(processed_sum)), 1));


		Assert::IsTrue(mth::geo3::Direction(orginal_sum.perpendicular(ma.x)).dot(orginal_sum.perpendicular(orto.x)) > 0);
		Assert::IsTrue(mth::geo3::Direction(orginal_sum.perpendicular(ma.y)).dot(orginal_sum.perpendicular(orto.y)) > 0);
		Assert::IsTrue(mth::geo3::Direction(orginal_sum.perpendicular(ma.z)).dot(orginal_sum.perpendicular(orto.z)) > 0);

	}
	TEST_METHOD(rotation_matrix_form_axis_turn)
	{
		mth::geo3::Axis_turn orginal{ mth::vec3{ 1,2,3 }, 0.25f };
		mth::geo3::Rotation_matrix rm = orginal;
		Assert::IsTrue(mth::geo3::is_non_reflected(rm));
		auto axis_turn = rm.axis_turn();

		Assert::IsTrue(mth::linear_algebra::are_similar(orginal.axis.vec(), axis_turn.axis.vec()));
		Assert::IsTrue(mth::are_similar(orginal.turn, axis_turn.turn));

	}
	TEST_METHOD(rotation_matrix_form_axis_turn_simple)
	{
		mth::geo3::Axis_turn orginal{ mth::vec3{ 0,1,0 }, 0.0f };
		mth::geo3::Rotation_matrix rm = orginal;
		Assert::IsTrue(mth::geo3::is_non_reflected(rm));
		auto axis_turn = rm.axis_turn();

		Assert::IsTrue(mth::are_similar(orginal.turn, axis_turn.turn));

	}
	TEST_METHOD(front_up_test)
	{
		mth::geo3::Front_up front_up;
		Assert::IsTrue(mth::linear_algebra::are_similar(front_up.right().vec(), mth::vec3(1,0,0)));

	}
};