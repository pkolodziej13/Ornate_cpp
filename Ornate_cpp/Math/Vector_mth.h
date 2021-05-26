#pragma once
#include <cmath>
#include <array>

#include <Utilities/loops.h>

#include "Float_utilities.h"
#include "Arithmetic_additable.h"
#include "Arithmetic_multiplicable.h"
#include "Multiplier_deduction.h"

#include "Vector_component.h"


namespace mth::linear_algebra
{

	template<class T, size_t I, class = std::make_index_sequence<I>>
	struct g_vec;

	template<class T, size_t ... i_v>
	struct g_vec_elements :vec_element<T, i_v>...
	{
		static constexpr size_t size = sizeof...(i_v);
		template<size_t i>
		using always_type = T;

		g_vec_elements() :vec_element<T, i_v>{}...
		{
			if constexpr (std::is_convertible_v<int, T>)
				((element<i_v>() = 0), ...);
		}
		g_vec_elements(const T& arg) :vec_element<T, i_v>{ arg }...
		{
		}
		g_vec_elements(const always_type<i_v> & ... elements) :vec_element<T, i_v>{ elements }...
		{}
		template<class T_specific, size_t size_specific, class = std::enable_if_t<std::is_convertible_v<T_specific, T>>>
		g_vec_elements(const g_vec<T_specific, size_specific>& vec) : g_vec_elements()
		{
			constexpr size_t min_size = size_specific < size ? size_specific : size;
			uti::for_index_sequence<min_size>([&](auto ind)
			{
				constexpr size_t index = std::decay_t<decltype(ind)>::value;
				this->element<index>() = (T)vec.element<index>();
			});
		}
		template<class T_specific, size_t size_specific, class = std::enable_if_t<std::is_convertible_v<T_specific, T>>>
		g_vec_elements(const std::array<T_specific, size_specific>& vec) : g_vec_elements()
		{
			constexpr size_t min_size = size_specific < size ? size_specific : size;
			uti::for_index_sequence<min_size>([&](auto ind)
			{
				constexpr size_t index = std::decay_t<decltype(ind)>::value;
				this->element<index>() = vec[index];
			});
		}
		template<class T_specific = T>
		std::array<T_specific, size> as_array()const
		{
			return std::array<T_specific, size>{T_specific{ element<i_v>() }...};
		}
		template<class T_specific>
		operator std::array<T_specific, size>()const
		{
			return as_array<T_specific>();
		}
		template<size_t i>
		inline T& element()
		{
			return static_cast<vec_element<T, i> *>(this)->element();
		}
		template<size_t i>
		inline const T& element()const
		{
			return static_cast<std::add_const_t<vec_element<T, i>> *>(this)->element();
		}
		const T& operator [](size_t i)const
		{
			return *((&element<0>()) + i);
		}
		T& operator [](size_t i)
		{
			return *((&element<0>()) + i);
		}
		T* data()
		{
			return &(this->element<0>());
		}
	};

	template<class T, size_t size_value, size_t ... I>
	struct g_vec<T, size_value, std::index_sequence<I...>>
		:g_vec_elements<T, I...>,
		mth::Arithmetic_additable<g_vec<T, size_value>, g_vec<T, size_value> >,
		mth::Arithmetic_additable<g_vec<T, size_value>, T >,
		mth::Arithmetic_multiplicable<g_vec<T, size_value>, g_vec<mth::Multiplier_of<T>, size_value> >,
		mth::Arithmetic_multiplicable<g_vec<T, size_value>, mth::Multiplier_of<T> >
	{
		using this_type = g_vec<T, size_value, std::index_sequence<I...>>;
		using arithmetic_additable = mth::Arithmetic_additable<this_type, this_type >;
		using arithmetic_scalar_additable = mth::Arithmetic_additable<this_type, T >;
		using arithmetic_multiplicable = mth::Arithmetic_multiplicable<this_type, g_vec<mth::Multiplier_of<T>, size_value, std::index_sequence<I...>> >;
		using arithmetic_scalar_multiplicable = mth::Arithmetic_multiplicable<this_type, mth::Multiplier_of<T> >;
		using base_type = g_vec_elements<T, I...>;
		using base_type::g_vec_elements;
		using base_type::element;
		static constexpr size_t size = sizeof...(I);
		using vec_type = g_vec<T, sizeof...(I)>;

		using arithmetic_additable::operator+;
		using arithmetic_additable::operator-;
		using arithmetic_additable::operator+=;
		using arithmetic_additable::operator-=;
		using arithmetic_scalar_additable::operator+;
		using arithmetic_scalar_additable::operator-;
		using arithmetic_scalar_additable::operator+=;
		using arithmetic_scalar_additable::operator-=;
		using arithmetic_multiplicable::operator*;
		using arithmetic_multiplicable::operator/;
		using arithmetic_multiplicable::operator*=;
		using arithmetic_multiplicable::operator/=;
		using arithmetic_scalar_multiplicable::operator*;
		using arithmetic_scalar_multiplicable::operator/;
		using arithmetic_scalar_multiplicable::operator*=;
		using arithmetic_scalar_multiplicable::operator/=;

		template<class Right_type, class Operation>
		static inline vec_type algebraic_pair_operation(const vec_type& a, const Right_type& b, Operation&& operation)
		{
			return vec_type{ operation(a.element<I>(),b)... };
		}
		template<class Right_type, class Operation>
		static inline vec_type algebraic_pair_operation(const vec_type& a, const g_vec<Right_type, size, std::index_sequence<I...>>& b, Operation&& operation)
		{
			return vec_type{ operation(a.element<I>(),b.element<I>())... };
		}

		T powlength()const
		{
			return ((this->element<I>() * this->element<I>()) + ...);
		}
		T length()const
		{
			return std::sqrt(powlength());
		}
		void normalize()
		{
			auto len = T(1) / length();
			((this->element<I>() *= len), ...);
		}
		vec_type normalized()const
		{
			vec_type result{ this->element<I>()... };
			result.normalize();
			return result;
		}
		bool operator ==(const vec_type& v)const
		{
			return ((this->element<I>() == v.element<I>()) && ...);
		}
		bool operator !=(const vec_type& v)
		{
			return ((this->element<I>() != v.element<I>()) || ...);
		}
		vec_type operator -()const
		{
			vec_type result;
			((result.element<I>() = -this->element<I>()), ...);
			return result;
		}
		inline T dot(const vec_type& b)const
		{
			return ((this->element<I>() * b.element<I>()) + ...);
		}
	};


	template<class T>
	g_vec<T, 3> cross(const g_vec<T, 3>& a, const g_vec<T, 3>& b)
	{
		return g_vec<T, 3>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}
	template<class T>
	g_vec<T, 2> cross(const g_vec<T, 2>& a)
	{
		return g_vec<T, 2>(a.y, -a.x);
	}

	template<class T, size_t I>
	auto normalize(const g_vec<T, I>& v)
	{
		return v.normalized();
	}
	template<class T, size_t I>
	auto dot(const g_vec<T, I>& v1, const g_vec<T, I>& v2)
	{
		return v1.dot(v2);
	}

	template<class T, size_t I>
	T length(const g_vec<T, I>& v)
	{
		return v.length();
	}

	template<class T, size_t I, size_t ...I_v>
	bool are_similar(const g_vec<T, I, std::index_sequence<I_v...>>& v1, const g_vec<T, I>& v2, T degree = T(0.01))
	{
		return (mth::are_similar((v1 - v2).length(),T(0.f), degree));
	}
}