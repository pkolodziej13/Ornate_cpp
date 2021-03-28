#pragma once
#include <algorithm>

namespace mth
{
	template<class T>
	struct Range
	{
		constexpr Range(const T& min = 0, const T& max = 0) :min(min), max(max) {}
		constexpr T size()const
		{
			return max - min;
		}
		constexpr T summed()const
		{
			return max + min;
		}
		bool is_inside(const T & v)const
		{
			return v >= min && v <= max;
		}
		T clamp(const T& v)const
		{
			return std::clamp<T>(v, min, max);
		}

		T min;
		T max;
	};
}