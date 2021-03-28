#pragma once
#include <cmath>

namespace mth
{
	template<size_t I>
	struct Floating_number_type;
	template<> struct Floating_number_type<1> { using type = float; };
	template<> struct Floating_number_type<2> { using type = double; };

	template<class T>
	constexpr inline size_t required_float_type()
	{
		if constexpr (std::is_same_v<T, float>)
			return 1;
		else if constexpr (std::is_same_v<T, double>)
			return 2;
		else
		{
			uint64_t max_value = []()
			{
				if constexpr (std::numeric_limits<T>::is_signed)
					return static_cast<uint64_t>(std::numeric_limits<T>::max()) + 1;
				else
					return std::numeric_limits<T>::max();
			}();

			uint64_t float_max_value = static_cast<uint64_t>(std::numeric_limits<float>::max());
			if (float_max_value < max_value)
				return 2;
			else
				return 1;
		}
	}
	template<class T>
	using Minimal_float = typename Floating_number_type<required_float_type<T>()>::type;

	template<std::floating_point   T>
	constexpr int sgn(const T& number)
	{
		constexpr T zero = static_cast<T>(0);
		return -(number < zero) + (number > zero);
	}
	template<std::floating_point T>
	constexpr T sqrt_temp(const T& number)
	{
		if constexpr (std::is_same_v<float, T>)
			return std::sqrtf(number);
		else if constexpr (std::is_same_v<double, T>)
			return std::sqrt(number);
	}


	inline bool are_similar(float a, float b, float precision = 0.01f)
	{
		return std::abs(a - b) < precision;
	}
	inline bool are_similar(double a, double b, double precision = 0.00001)
	{
		return std::abs(a - b) < precision;
	}
}