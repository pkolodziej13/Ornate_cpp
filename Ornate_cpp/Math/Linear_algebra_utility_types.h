#pragma once
#include "Vector_mth.h"
#include "Matrix.h"

namespace mth::linear_algebra
{
	using uint = size_t;
	using int2 = g_vec<int, 2>;
	using uint2 = g_vec<size_t, 2>;
	using vec1 = g_vec<float, 1>;
	using vec2 = g_vec<float, 2>;
	using vec3 = g_vec<float, 3>;
	using vec4 = g_vec<float, 4>;
	using mat1 = g_mat<float, 1, 1>;
	using mat2 = g_mat<float, 2, 2>;
	using mat3 = g_mat<float, 3, 3>;
	using mat4 = g_mat<float, 4, 4>;
}