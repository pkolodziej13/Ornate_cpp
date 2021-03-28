#pragma once
#include "Direction_3d.h"
#include "Front_up.h"
#include "Axis_3d.h"

namespace mth::geo3
{
	struct Plane
	{
		float distance(const vec3 & point)const
		{
			return normal.dot(point - center);
		}
		vec3 crossing_point(const Axis& axis)const
		{
			return axis.at_axis(distance(axis.center));
		}
		Direction normal;
		vec3 center;
	};
	struct Oriented_plane
	{
		float distance(const vec3& point)const
		{
			return front_up.front().dot(point - center);
		}
		vec3 crossing_point(const Axis& axis)const
		{
			return axis.at_axis(distance(axis.center));
		}
		vec2 crossing_point_at_plane(const Axis & axis)const
		{
			vec3 crossing = crossing_point(axis);
			vec3 from_center = crossing - center;

			return { front_up.right().dot(from_center),front_up.up().dot(from_center) };
		}

		Front_up front_up;
		vec3 center;
	};
}