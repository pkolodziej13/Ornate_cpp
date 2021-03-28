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

	template<class t, size_t I, class = std::make_index_sequence<I>>
	struct g_vec;

	template<class t, size_t ... I>
	struct g_vec_elements :vec_element<t, I>...
	{
		static constexpr size_t size = sizeof...(I);
		template<size_t i>
		using always_type = t;
		g_vec_elements() :vec_element<t, I>{}...
		{
			if constexpr (std::is_convertible_v<int, t>)
				((element<I>() = 0), ...);
		}
		g_vec_elements(const t& arg) :vec_element<t, I>{ arg }...
		{
		}
		g_vec_elements(const always_type<I> & ... elements) :vec_element<t, I>{ elements }...
		{}
		template<class t2, size_t size2, class = std::enable_if_t<std::is_convertible_v<t2, t>>>
		g_vec_elements(const g_vec<t2, size2>& vec) : g_vec_elements()
		{
			constexpr size_t min_size = size2 < size ? size2 : size;
			uti::for_index_sequence<min_size>([&](auto ind)
			{
				constexpr size_t index = std::decay_t<decltype(ind)>::value;
				this->element<index>() = (t)vec.element<index>();
			});
		}
		template<class t2, size_t size2, class = std::enable_if_t<std::is_convertible_v<t2, t>>>
		g_vec_elements(const std::array<t2, size2>& vec) : g_vec_elements()
		{
			constexpr size_t min_size = size2 < size ? size2 : size;
			uti::for_index_sequence<min_size>([&](auto ind)
			{
				constexpr size_t index = std::decay_t<decltype(ind)>::value;
				this->element<index>() = vec[index];
			});
		}
		template<class t2>
		operator std::array<t2, size>()const
		{
			return std::array<t2, size>{t2{ element<I>() }...};
		}
		template<size_t i>
		inline t& element()
		{
			return static_cast<vec_element<t, i> *>(this)->element();
		}
		template<size_t i>
		inline const t& element()const
		{
			return static_cast<std::add_const_t<vec_element<t, i>> *>(this)->element();
		}
		const t& operator [](size_t i)const
		{
			return *((&element<0>()) + i);
		}
		t& operator [](size_t i)
		{
			return *((&element<0>()) + i);
		}
		t* data()
		{
			return &(this->element<0>());
		}
	};

	template<class t, size_t siz, size_t ... I>
	struct g_vec<t, siz, std::index_sequence<I...>>
		:g_vec_elements<t, I...>,
		mth::Arithmetic_additable<g_vec<t, siz>, g_vec<t, siz> >,
		mth::Arithmetic_additable<g_vec<t, siz>, t >,
		mth::Arithmetic_multiplicable<g_vec<t, siz>, g_vec<mth::Multiplier_of<t>, siz> >,
		mth::Arithmetic_multiplicable<g_vec<t, siz>, mth::Multiplier_of<t> >
	{
		using this_type = g_vec<t, siz, std::index_sequence<I...>>;
		using arithmetic_additable = mth::Arithmetic_additable<this_type, this_type >;
		using arithmetic_scalar_additable = mth::Arithmetic_additable<this_type, t >;
		using arithmetic_multiplicable = mth::Arithmetic_multiplicable<this_type, g_vec<mth::Multiplier_of<t>, siz, std::index_sequence<I...>> >;
		using arithmetic_scalar_multiplicable = mth::Arithmetic_multiplicable<this_type, mth::Multiplier_of<t> >;
		using base_type = g_vec_elements<t, I...>;
		using base_type::g_vec_elements;
		using base_type::element;
		static constexpr size_t size = sizeof...(I);
		using vec_type = g_vec<t, sizeof...(I)>;

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
		static inline vec_type algebraic_pair_operation(const vec_type& a, const g_vec<Right_type, siz, std::index_sequence<I...>>& b, Operation&& operation)
		{
			return vec_type{ operation(a.element<I>(),b.element<I>())... };
		}

		t powlength()const
		{
			return ((this->element<I>() * this->element<I>()) + ...);
		}
		t length()const
		{
			return std::sqrt(powlength());
		}
		void normalize()
		{
			auto len = t(1) / length();
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
		inline t dot(const vec_type& b)const
		{
			return ((this->element<I>() * b.element<I>()) + ...);
		}
	};


	template<class t>
	g_vec<t, 3> cross(const g_vec<t, 3>& a, const g_vec<t, 3>& b)
	{
		return g_vec<t, 3>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}
	template<class t>
	g_vec<t, 2> cross(const g_vec<t, 2>& a)
	{
		return g_vec<t, 2>(a.y, -a.x);
	}

	template<class t, size_t I>
	auto normalize(const g_vec<t, I>& v)
	{
		return v.normalized();
	}
	template<class t, size_t I>
	auto dot(const g_vec<t, I>& v1, const g_vec<t, I>& v2)
	{
		return v1.dot(v2);
	}

	template<class t, size_t I>
	t length(const g_vec<t, I>& v)
	{
		return v.length();
	}

	template<class t, size_t I, size_t ...I_v>
	bool are_similar(const g_vec<t, I, std::index_sequence<I_v...>>& v1, const g_vec<t, I>& v2, t degree = t(0.01))
	{
		return (mth::are_similar((v1 - v2).length(),t(0.f), degree));
	}
}