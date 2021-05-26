#pragma once
#include "Linear_algebra_utility_types.h"
#include "Linear_algebra_utilities.h"

namespace mth
{
	using namespace mth::linear_algebra;

	template<class T>
	struct Vector_decomposition
	{
		static constexpr size_t dimension = 1;
		using element_type = T;
	};
	template<class T, size_t size_value>
	struct Vector_decomposition<g_vec<T, size_value>>
	{
		static constexpr size_t dimension = size_value;
		using element_type = T;
	};
}