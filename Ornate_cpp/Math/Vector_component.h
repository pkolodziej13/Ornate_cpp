#pragma once

namespace mth::linear_algebra
{
	template<class t, size_t I>
	struct vec_element
	{
		t e;
		t& element()
		{
			return e;
		}
		const t& element()const
		{
			return e;
		}
	};
	template<class t>
	struct vec_element<t, 0>
	{
		t x;
		t& element()
		{
			return x;
		}
		const t& element()const
		{
			return x;
		}
	};
	template<class t>
	struct vec_element<t, 1>
	{
		t y;
		t& element()
		{
			return y;
		}
		const t& element()const
		{
			return y;
		}
	};
	template<class t>
	struct vec_element<t, 2>
	{
		t z;
		t& element()
		{
			return z;
		}
		const t& element()const
		{
			return z;
		}
	};
	template<class t>
	struct vec_element<t, 3>
	{
		t w;
		t& element()
		{
			return w;
		}
		const t& element()const
		{
			return w;
		}
	};
}