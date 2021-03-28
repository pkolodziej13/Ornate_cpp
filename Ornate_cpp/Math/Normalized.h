#pragma once
#include "Fixed_point_number.h"
#include "Arithmetic_additable.h"
#include "Arithmetic_multiplicable.h"


namespace mth
{
	template <class internal_type>
	struct norm:
		Fixed_point_number<internal_type,std::numeric_limits<internal_type>::max()>,
		mth::Arithmetic_additable<norm<internal_type>, Minimal_float<internal_type> >,
		mth::Arithmetic_multiplicable<norm<internal_type>, Minimal_float<internal_type> >
	{
		using this_type = norm<internal_type>;
		using base_type = Fixed_point_number<internal_type, std::numeric_limits<internal_type>::max()>;
		using real_type = typename base_type::Minimal_reqired_float;
		using arithmetic_additable = mth::Arithmetic_additable<this_type, Minimal_float<internal_type> >;
		using arithmetic_multiplicable = mth::Arithmetic_multiplicable<this_type, Minimal_float<internal_type> >;

		norm() = default;
		norm( real_type  re)
			:base_type( re)
		{}
		norm(const base_type & fixed_point)
			:base_type(fixed_point)
		{}
		static this_type from_internal(const internal_type& t)
		{
			return this_type(base_type(0, t));
		}
		this_type & operator =(const real_type& f)
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
		void set_internal(const internal_type & in)
		{
			this->set( 0, in);
		}
		const internal_type & get_internal()const
		{
			return this->data();
		}
	};

	template<class t>
	struct is_norm:std::false_type{};
	template<class internal_type>
	struct is_norm<norm<internal_type>> :std::true_type{};
}