#pragma once
#include <Math/Plane.h>

namespace col
{
	struct Rectangular_plane
	{
		bool is_segment_crossing(const mth::vec3& a, const mth::vec3& b)const
		{
			if (mth::sgn(plane.distance(a)) !=
				mth::sgn(plane.distance(b)))
			{
				mth::geo3::Axis axis{ a,b };

				mth::vec2 on_wall = plane.crossing_point_at_plane(axis);
				return std::abs(on_wall.x) < center_to_edges.x &&
					std::abs(on_wall.y) < center_to_edges.y;
			}
			return false;
		}

		mth::geo3::Oriented_plane plane;
		mth::vec2 center_to_edges;
	};
}