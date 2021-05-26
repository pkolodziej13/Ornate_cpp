#include "pch.h"
#include "CppUnitTest.h"

#include <Math/Front_up.h>
#include <Math/Axis_3d.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace mth;
using namespace mth::geo3;

TEST_CLASS(Geometry3d_tests)
{
public:
	TEST_METHOD(Direction_perpedicular_simple)
	{
		mth::geo3::Direction dir = mth::vec3(0, 1, 0);
		mth::vec3 vec{ 1,1,0 };

		auto rigth = dir.cross(vec);

		auto result = dir.perpendicular(vec);

		Assert::IsTrue(mth::are_similar(dir.dot(result), 0));
		Assert::IsTrue(mth::are_similar(rigth.dot(result), 0));
		Assert::IsTrue(vec.dot(result) > 0);

	}
	TEST_METHOD(direction_reflect_simple)
	{
		mth::geo3::Direction dir = mth::vec3(0, 1, 0);
		mth::vec3 to_reflect{ 1,1,0 };
		mth::vec3 reflected = dir.reflect(to_reflect);
		Assert::IsTrue(mth::are_similar(dir.dot(to_reflect), -dir.dot(reflected)));
		Assert::IsTrue(dir.cross(to_reflect).dot(dir.cross(reflected)) > 0);
	}
	TEST_METHOD(direction_reflect_werid)
	{
		mth::geo3::Direction dir = mth::vec3(1, 2, 3);
		mth::vec3 to_reflect{ 0.1f,-3,-6 };
		mth::vec3 reflected = dir.reflect(to_reflect);
		Assert::IsTrue(mth::are_similar(dir.dot(to_reflect), -dir.dot(reflected)));
		Assert::IsTrue(dir.cross(to_reflect).dot(dir.cross(reflected)) > 0);
	}
	TEST_METHOD(cross_simple)
	{
		mth::geo3::Direction dir1 = mth::vec3(0, 0, 1);
		mth::geo3::Direction dir2 = mth::vec3(1, 0, 0);

		Assert::IsTrue(mth::linear_algebra::are_similar(dir1.cross(dir2), mth::vec3(0, 1, 0)));
	}
	TEST_METHOD(cross_ortogonal)
	{
		mth::geo3::Direction dir1 = mth::vec3(0, 0, 1);
		mth::geo3::Direction dir2 = mth::vec3(1, 2, 0);
		mth::vec3 result = dir1.cross(dir2);
		auto len = result.length();
		Assert::IsTrue(mth::are_similar(abs(len), 1.f));
	}
	TEST_METHOD(cross_ortogonal_longer)
	{
		mth::geo3::Direction dir1 = mth::vec3(0, 0, 1);
		mth::vec3 dir2 = mth::vec3(1, 2, 0);
		mth::vec3 result = dir1.cross(dir2);
		auto len = result.length();
		Assert::IsTrue(len > 1.f);
	}
	TEST_METHOD(cross_same)
	{
		mth::geo3::Direction dir1 = mth::vec3(0, 0, 1);
		mth::vec3 dir2 = mth::vec3(0, 0, 1);
		mth::vec3 result = dir1.cross(dir2);
		auto len = result.length();
		Assert::IsTrue(len == 0.0);
	}
	TEST_METHOD(cross_similar)
	{
		mth::geo3::Direction dir1 = mth::vec3(0, 0, 1);
		mth::geo3::Direction dir2 = mth::vec3(0, 0.01f, 0.999f);
		mth::vec3 dir3 = dir1.cross(dir2);
		mth::geo3::Direction result = dir3;
		Assert::IsTrue(mth::linear_algebra::are_similar(result.vec(), mth::vec3(-1, 0, 0)));
	}
	TEST_METHOD(cross_oposite)
	{
		mth::geo3::Direction dir1 = mth::vec3(0, 0, 1);
		mth::vec3 dir2 = mth::vec3(0, 0, -0.5f);
		mth::vec3 result = dir1.cross(dir2);
		auto len = result.length();
		Assert::IsTrue(len == 0.0);
	}
	TEST_METHOD(direction_from_angles_is_normalized)
	{
		mth::geo3::Direction dir(5.55f,7.77f,3.33f);
		float actual_length = dir.vec().length();
		Assert::IsTrue(are_similar(actual_length,1,0.000001f));
	}
	TEST_METHOD(Axis_distance)
	{

		Axis axis(vec3(1, 1, 1), vec3(0, 0, 0));

		Direction side_direction = Front_up(axis.direction).right();
		vec3 checked_point = side_direction.vec() + axis.direction * 7.77f;

		Assert::IsTrue(are_similar(axis.distance(checked_point),1.f));
	}
	TEST_METHOD(Axis_translation)
	{
		using namespace mth;
		using namespace mth::geo3;

		Axis axis(vec3(1, 1, 1), vec3(0, 0, 0));

		Direction side_direction = Front_up(axis.direction).right();
		vec3 checked_point = side_direction.vec() + axis.direction*5.55f;

		Assert::IsTrue(linear_algebra::are_similar(axis.translation(checked_point), side_direction.vec()));
	}
};