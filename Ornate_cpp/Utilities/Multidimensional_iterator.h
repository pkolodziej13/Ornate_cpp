#pragma once
#include <array>

#include "loops.h"

namespace uti
{
	template<size_t dim>
	struct multidimensional_Iterator
	{
		std::array< size_t, dim> size;
		std::array< size_t, dim> start;
		std::array<size_t, dim > acces_helper;
		size_t sum;
		size_t ii;
		multidimensional_Iterator()
		{

		}
		multidimensional_Iterator(std::array<size_t, dim> siz)
		{
			start.fill(0);
			ii = 0;
			size = siz;
			sum = 1;
			for (auto a : siz)
				sum *= a;
			acces_helper[0] = 1;
			for (size_t i = 1; i < dim; i++)
			{
				acces_helper[i] = siz[i - 1] * acces_helper[i - 1];
			}
		}
		multidimensional_Iterator(std::array<size_t, dim> _start, std::array<size_t, dim> siz)
		{
			start = _start;
			ii = 0;
			size = siz;
			sum = 1;
			for (auto a : siz)
				sum *= a;
			acces_helper[0] = 1;
			for (size_t i = 1; i < dim; i++)
			{
				acces_helper[i] = siz[i - 1] * acces_helper[i - 1];
			}
		}
		multidimensional_Iterator& begin()
		{
			ii = 0;
			return *this;
		}
		multidimensional_Iterator& end()
		{
			return *this;
		}
		inline void operator++()
		{
			ii++;
		}
		inline std::array<size_t, dim> operator*()
		{
			size_t i = ii;
			std::array<size_t, dim> to_return;
			for (size_t ii = dim; ii > 1; ii--)
			{
				to_return[ii - 1] = i / acces_helper[ii - 1] + start[ii - 1];
				i = i % acces_helper[ii - 1];
			}
			to_return[0] = i + start[0];
			return to_return;
		}
		inline bool operator !=(const  multidimensional_Iterator& b)
		{
			return ii < sum;
		}


	};
}