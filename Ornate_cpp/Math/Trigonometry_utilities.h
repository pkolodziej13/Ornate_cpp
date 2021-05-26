#pragma once
#include <algorithm>

#include "Trigonometry.h"

namespace mth
{
	inline float law_of_cosines_angle(float a, float b, float c)
	{
		return std::acos(std::clamp((c * c - a * a - b * b) / (-2.f * a * b), -1.f, 1.f));
	}
	inline float law_of_sines_angle(float a, float b, float angle_a)
	{
		return std::asin(std::min<float>(1.f, (b * std::sin(angle_a)) / a));
	}
}