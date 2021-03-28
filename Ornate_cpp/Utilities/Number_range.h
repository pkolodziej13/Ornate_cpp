#pragma once
#include "Range_for_loop.h"

namespace uti
{
	struct Number_loop_range
	{
		Number_loop_range(size_t size)
			:size{ size }
		{}
		Number_loop_range(size_t i, size_t size)
			:i{ i }, size{ size }
		{}
		bool finished()const
		{
			return i >= size;
		}
		size_t current_element()const
		{
			return i;
		}
		void increment()
		{
			++i;
		}
		void finish_after_increment()
		{
			i = size;
		}
	private:
		size_t i = 0;
		size_t size = 0;
	};

	using Number_range = Range_for_loop<Number_loop_range>;

}