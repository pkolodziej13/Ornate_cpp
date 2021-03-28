#include "pch.h"
#include "CppUnitTest.h"

#include <Collisions\Box.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Collisions_engine_tests)
{
public:
	TEST_METHOD(ball_beyond_box_planes)
	{
		col::Ball ball;
		ball.center = mth::vec3(0, 0, 0);
		ball.radius = 1;

		col::Box box;
		box.coordines_as_matrix.translate({ 10,0,0 });
		Assert::IsTrue(col::is_ball_beyond_box_planes(box, ball));

		box.coordines_as_matrix.translate({ -10,0,0 });
		Assert::IsFalse(col::is_ball_beyond_box_planes(box, ball));
	}
	TEST_METHOD(box_colliding_with_ball)
	{
		col::Ball ball;
		ball.center = mth::vec3(0, 0, 0);
		ball.radius = 1;

		col::Box box;
		box.coordines_as_matrix.translate({ 10,0,0 });
		Assert::IsFalse(col::are_colliding(box, ball));

		box.coordines_as_matrix.translate({ -10,0,0 });
		Assert::IsTrue(col::are_colliding(box, ball));
	}
};