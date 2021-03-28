#pragma once
#include <limits>
#include <cstdint>
namespace mth
{
	template<class T>
	constexpr T positive_negative(bool is_positive)
	{
		return static_cast<T>(-1 + 2 * is_positive);
	}
	template<std::signed_integral  T>
	constexpr int sgn(const T&number)
	{
		constexpr T zero = static_cast<T>(0);
		return -(number < zero) + (number > zero);
	}

	template<class T>
	constexpr T ceil_to_number(T x, T to_ceil)
	{
		auto round_down = (x / to_ceil) * to_ceil;
		return round_down + to_ceil * (x > round_down);
	}

	template<class T, class Less_condition>
	inline constexpr T binary_search(T x, T lo, T hi, Less_condition&& less_condition)noexcept
	{
		T result = (hi + lo + 1) / 2;
		while (lo != hi)
		{
			if (less_condition(x, result))
				hi = result - 1;
			else
				lo = result;
			result = (hi + lo + 1) / 2;
		}
		return result;
	}
	template<class T, T lo, T hi, class Less_condition>
	inline constexpr T binary_const_search(T x, Less_condition&& less_condition)noexcept
	{
		if constexpr (lo == hi)
		{
			return lo;
		}
		else
		{
			constexpr T mid = (hi + lo + 1) / 2;
			if (less_condition(x, mid))
				return binary_const_search<T, lo, mid - 1>(x, less_condition);
			else
				return binary_const_search<T, mid, hi>(x, less_condition);
		}
	}

	namespace compile_time
	{

		template<class T>
		constexpr T floorlog2(T x)
		{
			if (x <= 0)
				return std::numeric_limits<T>::min();

			T result = 0;
			while (x > 1)
			{
				x = x >> 1;
				result++;
			}
			return result;
		}
		template<class T>
		constexpr T ceillog2(T x)
		{
			if constexpr (std::is_signed_v<T>)
			{
				if (x <= 0)
					return std::numeric_limits<T>::min();
				if (x == 1)
					return 0;
			}
			else
			{
				if (x <= 1)
					return std::numeric_limits<T>::min();
			}

			x -= 1;
			T result = 0;
			while (x > 1)
			{
				x = x >> 1;
				result++;
			}
			return result + 1;
		}
		template<class T>
		constexpr T bit_width(T x)
		{
			if constexpr (std::is_signed_v<T>)
			{
				bool needs_sign = x < 0;
				x += needs_sign;
				x *= positive_negative<T>(!needs_sign);

				T result = 0;
				while (x != 0)
				{
					x = x >> 1;
					result++;
				}
				return result + needs_sign;
			}
			else
			{
				T result = 0;
				while (x != 0)
				{
					x = x >> 1;
					result++;
				}
				return result;
			}
		}
		template<class T>
		constexpr T bit_width_binary(T x)noexcept
		{
			if (x == 0)
				return 0;
			constexpr T max_value = sizeof(T) * 8;
			if constexpr (std::is_signed_v<T>)
			{
				bool needs_sign = x < 0;
				x += needs_sign;
				x *= positive_negative<T>(!needs_sign);

				return binary_const_search<T, 0, max_value>(x,
					[](T x, T y) {return x >> y == 0; }) + 1 + needs_sign;
			}
			else
			{
				return binary_const_search<T, 0, max_value>(x,
					[](T x, T y) {return x >> y == 0; }) + 1;
			}
		}

		template<class T>
		constexpr T sqrt(T x_arg)
		{
			T x = x_arg, lo = 0, hi = x_arg / 2 + 1;

			while (lo != hi)
			{
				const T mid = (lo + hi + 1) / 2;

				if (x / mid < mid)
					hi = mid - 1;
				else
					lo = mid;
			}
			return lo;
		}
		template<class T>
		constexpr T sqrt_binary(T x)
		{
			T r = bit_width_binary<T>(x);
			constexpr T max_result = std::numeric_limits<T>::max() >> (sizeof(T) * 4);
			constexpr size_t max_result_bits = bit_width_binary<T>(max_result);
			T lo = max_result >> (max_result_bits - (r + 1) / 2 + 1);
			T hi = max_result >> (max_result_bits - (r + 1) / 2);

			return binary_search<T>(x, lo, hi,
				[](T x, T mid) {return x / mid < mid; });
		}
	}


}