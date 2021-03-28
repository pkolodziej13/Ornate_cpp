#pragma once
#include "Used_linear_algebra.h"
#include "Rotation_matrix_2d.h"
#include "Rotation_matrix_3d.h"

namespace mth
{
	template<class t = float, size_t dim = 3>
	struct Model_matrix :g_mat<t, dim + 1, dim + 1>
	{
		static_assert(dim == 2 || dim == 3);
		using base_type = g_mat<t, dim + 1, dim + 1>;
		using this_type = Model_matrix<t, dim>;
		using vec_type = g_vec<t, dim>;
		using complete_vec = g_vec<t, dim + 1>;
		using rot_mat_type = std::conditional_t<dim==2,geo2::Rotation_matrix, geo3::Rotation_matrix>;

		using base_type::g_mat;
		Model_matrix() = default;
		Model_matrix(const base_type& mat) :base_type{ mat } {}
		Model_matrix(const vec_type& translation_a)
		{
			translate(translation_a);
		}
		Model_matrix(const rot_mat_type& rotation_a)
		{
			rotate(rotation_a);
		}
		Model_matrix(const t& scale_a)
		{
			scale(scale_a);
		}
		Model_matrix(const vec_type& translation_a, const rot_mat_type& rotation_a, const vec_type& scale_a)
		{
			translate(translation_a);
			rotate(rotation_a);
			scale(scale_a);
		}
		this_type& translate(const vec_type& translation_a)
		{
			base_type trans;
			trans.element<dim>() = translation_a;
			trans.element<dim>().element<dim>() = 1;
			(*this) = (*this) * trans;

			return *this;
		}
		vec_type translation()
		{
			return this->element<dim>();
		}
		this_type& rotate(const rot_mat_type& rotation_a)
		{
			(*this) = (*this) * base_type(rotation_a.matrix());
			return *this;
		}
		rot_mat_type rotation()
		{
			auto scal = scale();
			g_mat<t, dim, dim > result = *this;
			for_i(dim)
			{
				result[i] /= scal[i];
			}
			return result;
		}
		this_type& scale(const vec_type& scale_a)
		{
			base_type bas;
			for_i(dim)
			{
				bas[i][i] = scale_a[i];
			}
			(*this) = (*this) * bas;
			return *this;
		}
		vec_type scale()
		{
			vec_type result;
			for_i(dim)
			{
				result[i] = vec_type((*this)[i]).length();
			}
			return result;
		}
		vec_type operator*(const vec_type& to_transform)const
		{
			return matrix() * vec4(to_transform.x, to_transform.y, to_transform.z, 1);
		}
		vec_type multiply_direction(const vec_type& to_transform)const
		{
			return matrix() * vec4(to_transform.x, to_transform.y, to_transform.z, 0);
		}
		this_type operator*(const this_type& to_transform)const
		{
			return { matrix() * to_transform.matrix() };
		}
		this_type inversed()const
		{
			return this_type{ matrix().inversed() };
		}

		const base_type& matrix()const
		{
			return *this;
		}
	};
}