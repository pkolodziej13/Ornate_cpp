#pragma once
#include <limits>
namespace uti
{
	template<class T>
	struct Index
	{
		static constexpr T inactive = std::numeric_limits<T>::max();
		Index() = default;
		constexpr Index(T value):value(value)
		{}
		constexpr Index<T> & operator =(T value)
		{
			this->value = value;
			return *this;
		}
		constexpr void reset()
		{
			value = inactive;
		}
		constexpr bool actie()
		{
			return value != inactive;
		}
		constexpr bool empty()
		{
			return value = inactive;
		}
		T value{ inactive };
	};
}