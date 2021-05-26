#pragma once

namespace mth::linear_algebra
{
	template<class T, size_t I>
	struct vec_element
	{
		T& element()
		{
			return e;
		}
		const T& element()const
		{
			return e;
		}

		T e;
	};
	template<class T>
	struct vec_element<T, 0>
	{
		T& element()
		{
			return x;
		}
		const T& element()const
		{
			return x;
		}

		T x;
	};
	template<class T>
	struct vec_element<T, 1>
	{
		T& element()
		{
			return y;
		}
		const T& element()const
		{
			return y;
		}

		T y;
	};
	template<class T>
	struct vec_element<T, 2>
	{
		T& element()
		{
			return z;
		}
		const T& element()const
		{
			return z;
		}

		T z;
	};
	template<class T>
	struct vec_element<T, 3>
	{
		T& element()
		{
			return w;
		}
		const T& element()const
		{
			return w;
		}

		T w;
	};
}