#pragma once
#include <Types/Utility.h>
#include "Float_utilities.h"
#include "Range.h"
#include "Used_linear_algebra.h"
#include "Model_matrix.h"

namespace mth
{ 
	template<class T>
	struct Rectangular_frame :Range<g_vec<T,2>>
	{
		using this_type = Rectangular_frame<T>;
		using vec_type = g_vec<T, 2>;
		using floating_type = Minimal_float<T>;
		using Range<g_vec<T, 2>>::size;

		Rectangular_frame(vec_type start = vec_type(0, 0), vec_type size = vec_type(1, 1)) :Range<vec_type>(start, start + size) {}

		void set_size(vec_type new_size)
		{
			this->max = this->min + new_size;
		}
		void move(vec_type to_move)
		{
			this->max += to_move;
			this->min += to_move;
		}
		template<size_t D = 0>
		vec_type unit_size()const
		{
			return size() / size().element<D>();
		}
		template<size_t D = 0>
		this_type to_unit(this_type real)
		{
			float inv_scalar = 1.f / size.element<D>();
			return { (this->min - real.min) / inv_scalar, real.size() / inv_scalar };
		}
		template<size_t D= 0>
		this_type from_unit(this_type unit)const 
		{
			floating_type scalar = size().element<D>();

			return { this->min + unit.min*scalar, unit.size()*scalar };
		}
		template<size_t D= 0>
		g_vec<floating_type, 2> proportional()
		{
			g_vec<floating_type, 2> floating_size = size();
			return floating_size / floating_size.element<D>();
		}
		g_vec<floating_type, 2> proportional_to_one()
		{
			g_vec<floating_type, 2> floating_size = size();
			return floating_size / std::max<floating_type>(floating_size.x, floating_size.y);
		}
		g_vec<floating_type, 2> proportional_from_one()
		{
			g_vec<floating_type, 2> floating_size = size();
			return floating_size / std::min<floating_type>(floating_size.x, floating_size.y);
		}
		template<class T_specific>
		auto operator*(const g_vec<T_specific,2> & rec)const
		{
			using higher_type = std::decay_t<decltype(T() + T_specific())>;
			using floating_type_c = typ::type_of<floating_type_deductor<higher_type>>;
			return Rectangular_frame<floating_type_c>( this->min*rec ,size()*rec );
		}
		template<class T_specific>
		auto operator*=(const g_vec<T_specific,2> & rec)
		{
			using higher_type = decltype(T{} + T_specific{});
			using floating_type_c = typ::type_of<floating_type_deductor<higher_type>>;
			this->min *= rec;
			this->max *= rec;
			return *this;
		}
		static this_type signed_frame()
		{
			return { {-1,-1},{2,2} };
		}


		g_mat<floating_type,4,4> matrix_of_symetric()const
		{
			Model_matrix<floating_type,3> result;
			auto siz = size();
			return result.translate(vec_type(-1)+siz+this->min*2).scale({ siz.x, siz.y, 1 });
		}

		template<size_t D = 0>
		this_type relative_to_unit_square(const this_type & unit)const
		{
			vec_type rev = vec_type(1) / size();
			return unit * rev;
		}

		g_mat<floating_type, 4, 4> matrix_relative(const this_type & unit)
		{
			Model_matrix<floating_type, 3> result;
			vec_type siz = this->size();
			vec_type position_div = unit.min - this->min;
			vec_type move = position_div / siz;
			result.translate(vec3(move.x, move.y, 1)).scale(unit.size()/siz);
			return result;
		}
		template<size_t D = 0>
		g_mat<floating_type, 4, 4> matrix_relative_port(
			const this_type & relative,
			const this_type & relative_port= this_type({ -1,-1 }, { 2, 2 }))const 
		{
			Model_matrix<floating_type, 3> result;

			vec2 center_in_relative = (-relative_port.min) / relative_port.size();
			result
				.translate(relative.min)
				.scale(relative.size())
				.translate(center_in_relative)
				.scale(vec3(1)/vec3(relative_port.size().x, relative_port.size().y,1));
			return result;
		}
		bool operator ==(const this_type & mat)const
		{
			return (this->min == mat.min)&&(this->max == mat.max);
		}

	};
}