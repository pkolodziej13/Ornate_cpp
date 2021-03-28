#pragma once
#include <vector>

#include "Multiarray_orientation.h"

namespace agg
{
	template<class T, size_t dim>
	struct Multiarray:private Multiarray_orientation<dim>
	{
		void resize(std::array<size_t, dim> siz)
		{
			Multiarray_orientation<dim>::resize(siz);

			data_vector.resize(Multiarray_orientation<dim>::internal_size());
		}
		using Multiarray_orientation<dim>::size;
		size_t internal_size()
		{
			return data_vector.size();
		}
		using Multiarray_orientation<dim>::internal_size;
		using Multiarray_orientation<dim>::internal_index;
		using Multiarray_orientation<dim>::external_index;

		T& operator [](const std::array<size_t, dim>& i)
		{
			size_t in = 0;
			for (size_t ii = 0; ii < dim; ii++)
			{
				in += this->access_helper[ii] * i[ii];
			}
			return data_vector[in];
		}
		const T& operator [](const std::array<size_t, dim>& i)const
		{
			size_t in = 0;
			for (size_t ii = 0; ii < dim; ii++)
			{
				in += this->access_helper[ii] * i[ii];
			}
			return data_vector[in];
		}
		T& get_internal(size_t i)
		{
			return data_vector[i];
		}
		const T& get_internal(size_t i)const
		{
			return data_vector[i];
		}
		T* data()
		{
			return data_vector.data();
		}
		const T* data()const
		{
			return data_vector.data();
		}

	private:
		std::vector<T> data_vector;
	};
}

