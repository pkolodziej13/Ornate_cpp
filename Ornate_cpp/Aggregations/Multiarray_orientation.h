#pragma once
#include <array>

namespace agg
{
	template<size_t dimension>
	struct Multiarray_orientation
	{
		Multiarray_orientation()
		{
			sizes.fill(0);
			access_helper.fill(1);
		}
		Multiarray_orientation(const std::array<size_t, dimension>& siz)
		{
			resize(siz);
		}
		void resize(const std::array<size_t, dimension>& siz)
		{
			sizes = siz;
			access_helper[0] = 1;
			for (size_t i = 1; i < dimension; i++)
			{
				access_helper[i] = siz[i - 1] * access_helper[i - 1];
			}
		}
		std::array<size_t, dimension> size()const
		{
			return sizes;
		}
		size_t internal_size()
		{
			size_t result = 1;
			for (auto a : sizes)
				result *= a;
			return result;
		}
		size_t internal_index(const std::array<size_t, dimension>& i)
		{
			size_t in = 0;
			for (size_t ii = 0; ii < dimension; ii++)
			{
				in += access_helper[ii] * i[ii];
			}
			return in;
		}
		std::array<size_t, dimension> external_index(size_t i)
		{
			std::array<size_t, dimension> to_return;
			for (size_t ii = dimension; ii > 1; ii--)
			{
				to_return[ii - 1] = i / access_helper[ii - 1];
				i = i % access_helper[ii - 1];
			}
			to_return[0] = i;
			return to_return;
		}
	protected:
		std::array<size_t, dimension> sizes;
		std::array<size_t, dimension > access_helper;
	};
}