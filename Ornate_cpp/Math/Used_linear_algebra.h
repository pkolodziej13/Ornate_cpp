#pragma once
#include "Linear_algebra_utility_types.h"
#include "Linear_algebra_utilities.h"

namespace mth
{
	using namespace mth::linear_algebra;

	template<class t>
	struct Vector_decomposition
	{
		static constexpr size_t dimension = 1;
		using element_type = t;
	};
	template<class t, size_t dim>
	struct Vector_decomposition<g_vec<t, dim>>
	{
		static constexpr size_t dimension = dim;
		using element_type = t;
	};
}