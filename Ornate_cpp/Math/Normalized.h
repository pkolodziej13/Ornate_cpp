#pragma once
#include "Fixed_point_number.h"
#include "Arithmetic_additable.h"
#include "Arithmetic_multiplicable.h"

namespace mth
{
	template <class Internal_type>
	struct Normalized:
		Fixed_point_number<Internal_type,std::numeric_limits<Internal_type>::max()>,
		mth::Arithmetic_additable<Normalized<Internal_type>, Minimal_float<Internal_type> >,
		mth::Arithmetic_multiplicable<Normalized<Internal_type>, Minimal_float<Internal_type> >
	{
		using this_type = Normalized<Internal_type>;
		using base_type = Fixed_point_number<Internal_type, std::numeric_limits<Internal_type>::max()>;
		using real_type = typename base_type::Minimal_reqired_float;
		using arithmetic_additable = mth::Arithmetic_additable<this_type, Minimal_float<Internal_type> >;
		using arithmetic_multiplicable = mth::Arithmetic_multiplicable<this_type, Minimal_float<Internal_type> >;

		Normalized() = default;
		Normalized(real_type const& re)
			:base_type( re)
		{}
		Normalized(base_type const & fixed_point)
			:base_type(fixed_point)
		{}
		static this_type from_internal(const Internal_type& t)
		{
			return this_type(base_type(0, t));
		}
		this_type & operator =(real_type const& f)
		{
			this->set(f);
			return *this;
		}

		using arithmetic_additable::operator+;
		using arithmetic_additable::operator-;
		using arithmetic_additable::operator+=;
		using arithmetic_additable::operator-=;
		using arithmetic_multiplicable::operator*;
		using arithmetic_multiplicable::operator/;
		using arithmetic_multiplicable::operator*=;
		using arithmetic_multiplicable::operator/=;
		template<class Operation>
		static this_type algebraic_pair_operation(const this_type& a, const real_type& b, Operation&& operation)
		{
			return operation(static_cast<const base_type &>(a), b);
		}
		void set_internal(const Internal_type & in)
		{
			this->set( 0, in);
		}
		const Internal_type & get_internal()const
		{
			return this->data();
		}
	};

	template<class T>
	struct is_normalized :std::false_type{};
	template<class Internal_type>
	struct is_normalized<Normalized<Internal_type>> :std::true_type{};
}