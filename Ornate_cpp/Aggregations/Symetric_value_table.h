#pragma once
#include <array>
namespace agg
{
	template<class T,size_t I>
	struct Symetric_value_table
	{
		constexpr Symetric_value_table() {}
		constexpr Symetric_value_table(const T& val)
		{
			for (auto& a : data)
				for (auto& b : a)
					b = val;
		}
		constexpr void set(size_t i,size_t j,const T & value)
		{
			data[i][j] = value;
			data[j][i] = value;
		}
		constexpr const T & get(size_t i, size_t j)const
		{
			return data[i][j];
		}
		template<class Lambda>
		constexpr void for_each(Lambda&& lambda)const
		{
			for (size_t i = 0; i < I; ++i)
				for (size_t j = i; j < I; ++j)
					lambda(data[i][j]);
		}
	private:
		std::array<std::array<T, I>, I> data;
	};
}