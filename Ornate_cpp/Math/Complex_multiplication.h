#pragma once
#include "Vector_mth.h"

namespace mth::linear_algebra
{
	template<class T>
	g_vec<T, 2> complex_multiplication(const g_vec<T, 2>& a, const g_vec<T, 2>& b)
	{
		return g_vec<T, 2>(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
	}
	template<class T>
	g_vec<T, 4> complex_multiplication(const g_vec<T, 4>& a, const g_vec<T, 4>& b)
	{
		return g_vec<T, 4>(
			a.x * b.x - a.y * b.y - a.z * b.z - a.w * b.w,
			a.y * b.x + a.x * b.y + a.w * b.z - a.z * b.w,
			a.z * b.x - a.w * b.y + a.x * b.z + a.y * b.w,
			a.w * b.x + a.z * b.y - a.y * b.z + a.x * b.w);
	}
}