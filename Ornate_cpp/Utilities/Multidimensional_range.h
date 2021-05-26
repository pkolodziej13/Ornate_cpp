#pragma once
#include <Aggregations/Multiarray_orientation.h>
#include <Math/Used_linear_algebra.h>

#include "loops.h"
#include "Range_for_loop.h"

namespace uti
{
	template<size_t dimensions>
	struct Multidimensional_loop_range
	{
		using vec_type = mth::g_vec<size_t, dimensions>;
		using array_type = mth::g_vec<size_t, dimensions>;
		Multidimensional_loop_range()
		{
			orientation.resize(vec_type(0));
			start = vec_type(0);
			i = 0;
		}
		Multidimensional_loop_range(vec_type const & size_to_set)
		{
			orientation.resize(size_to_set.as_array());
			total_elements_number = orientation.internal_size();
			start = vec_type(0);
			i = 0;
		}
		Multidimensional_loop_range(vec_type const& start_to_set, vec_type const& size_to_set)
		{
			orientation.resize((size_to_set- start_to_set).as_array());
			total_elements_number = orientation.internal_size();
			start = start_to_set;
			i = 0;
		}

		bool finished()const
		{
			return i >= total_elements_number;
		}
		array_type current_element()const
		{
			return (start + orientation.external_index(i)).as_array();
		}
		void increment()
		{
			++i;
		}
		void finish_after_increment()
		{
			i = total_elements_number;
		}

	private:
		agg::Multiarray_orientation<dimensions> orientation;
		size_t total_elements_number;

		size_t i;

		vec_type start;
	};

	template<size_t dimensions>
	using Multidimensional_range = Range_for_loop<Multidimensional_loop_range<dimensions>>;
}