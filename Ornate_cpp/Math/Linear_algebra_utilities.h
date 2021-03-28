#pragma once
#include "Used_linear_algebra.h"
#include "Range.h"

namespace mth::linear_algebra
{
	template <class T>
	g_mat<T, 4, 4> orthographic_projection(const Range<T> & left_right, const Range<T>& bottom_up , const Range<T>& near_far)
	{
		g_mat<T, 4, 4> result;
		result[0][0] = static_cast<T>(2) / left_right.size();
		result[1][1] = static_cast<T>(2) / bottom_up.size();
		result[2][2] = -static_cast<T>(2) / near_far.size();
		result[3][0] = -left_right.summed() / left_right.size();
		result[3][1] = -bottom_up.summed() / bottom_up.size();
		result[3][2] = -near_far.summed() / near_far.size();
		return result;
	}

	template <typename T>
	g_mat<T, 4, 4> perspective_projection(const Range<T>& left_right, const Range<T>& bottom_up, const Range<T>& near_far)
	{
		g_mat<T, 4, 4> result;
		result[0][0] = (static_cast<T>(2) * near_far.min) / left_right.size();
		result[1][1] = (static_cast<T>(2) * near_far.min) / bottom_up.size();
		result[2][0] = left_right.summed() / left_right.size();
		result[2][1] = bottom_up.summed() / bottom_up.size();
		result[2][2] = -near_far.summed() / near_far.size();
		result[2][3] = static_cast<T>(-1);
		result[3][2] = -(static_cast<T>(2) * near_far.min * near_far.max) / near_far.size();
		return result;
	}

	template<class T, class Scalar>
	inline T lerp(const T& a, const T& b, const Scalar& degree)
	{
		return a * (static_cast<T>(1) - degree) + b * (degree);
	}
}


