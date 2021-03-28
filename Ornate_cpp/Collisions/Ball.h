#pragma once
#include <Math/Used_linear_algebra.h>
#include <Math/Plane.h>

namespace col
{
	struct Ball
	{
		bool operator ==(const Ball& to_compare)const
		{
			return center == to_compare.center && radius == to_compare.radius;
		}
		mth::vec3 center{ 0,0,0 };
		float radius = 1;
	};

	inline bool are_colliding(const Ball& ball1, const Ball& ball2)
	{
		float distance_between_centers = (ball1.center - ball2.center).length();
		return (ball1.radius + ball2.radius)> distance_between_centers;
	}
}