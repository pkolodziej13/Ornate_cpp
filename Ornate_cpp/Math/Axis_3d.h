#pragma once
#include "Direction_3d.h"

namespace mth::geo3
{
	struct Axis
	{
		Axis(const Direction & direction,const vec3 & center)
			:direction(direction), center(center)
		{}
		Axis(const vec3& a, const vec3& b)
			:direction(b-a), center(a)
		{}
		vec3 at_axis(float x)const
		{
			return center + direction * x;
		}
		float distance(vec3 v)const
		{
			mth::vec3 from_center = v - center;
			Direction side = mth::cross(from_center, direction.vec());
			return mth::cross(direction.vec(), side.vec()).dot(from_center);
		}
		vec3 translation(vec3 v)const
		{
			mth::vec3 from_center = v - center;
			Direction side = mth::cross(from_center, direction.vec());
			return mth::cross(direction.vec(), side.vec());
		}

		Direction direction;
		vec3 center;
	};
}