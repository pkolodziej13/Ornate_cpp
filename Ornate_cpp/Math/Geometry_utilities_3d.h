#pragma once
#include "Trigonometry_utilities.h"
#include "Used_linear_algebra.h"


namespace mth::geo3
{
	inline float triangleField(const vec3& a, const vec3& b, const vec3& c)
	{
		float aa = (a - b).length();
		float bb = (a - c).length();
		float cc = (c - b).length();
		return std::sqrt((aa + bb + cc) * (aa + bb - cc) * (aa - bb + cc) * (-aa + bb + cc)) / 4;

	}

	inline std::array<float, 3> get_angles(vec3 a, vec3 b, vec3 c)
	{
		std::array<float, 3> to_return;
		std::array<float, 3> sides = { (b - a).length(),(c - b).length(),(a - c).length() };
		to_return[0] = law_of_cosines_angle(sides[2], sides[0], sides[1]);
		to_return[1] = law_of_sines_angle(sides[1], sides[2], to_return[0]);
		to_return[2] = pi - to_return[0] - to_return[1];

		return to_return;

	}
}