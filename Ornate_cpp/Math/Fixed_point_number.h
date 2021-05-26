#pragma once
#include <limits>
#include <compare>

#include "Integer_construction.h"
#include "Float_utilities.h"

namespace mth
{
	template<class T, T unit_value>
	struct Fixed_point_number
	{
		using this_type = Fixed_point_number<T, unit_value>;
		using Numeric_limits = std::numeric_limits<T>;
		using Operational_type = typename Integer_construction<Numeric_limits::is_signed, CHAR_BIT* sizeof(T)*2>::type;
		using Minimal_reqired_float = Minimal_float<T>;

		constexpr Fixed_point_number() = default;
		constexpr Fixed_point_number(const Minimal_reqired_float& fractional)
			:value{ static_cast<T>( value_from_floating(fractional)) }
		{}
		constexpr Fixed_point_number(const T& int_part, const T& fractional = 0)
			:value{ static_cast<T>(int_part * unit_value + fractional) }
		{}

		constexpr static this_type add(const this_type& a, const this_type& b)
		{
			return { 0,static_cast<T>( a.value + b.value) };
		}
		constexpr static this_type subtract(const this_type& a, const this_type& b)
		{
			return { 0,static_cast<T>(a.value - b.value )};
		}
		struct Operational_converted
		{
			Operational_converted(const this_type& to_copy)
				:value{ to_copy.value }
			{}
			Operational_converted(const Minimal_reqired_float& to_copy)
				:value{ static_cast<Operational_type>(to_copy * static_cast<Minimal_reqired_float>(unit_value)) }
			{}
			Operational_type value;
		};
		constexpr static this_type multiply(const this_type& a, const Operational_converted& b)
		{
			Operational_type operational = a.value;
			operational *= b.value;
			operational /= unit_value;
			return { 0, static_cast<T>(operational) };
		}
		constexpr static this_type divide(const this_type& a, const Operational_converted& b)
		{
			Operational_type operational = a.value;
			operational *= unit_value;
			operational /= b.value;
			return { 0, static_cast<T>(operational) };
		}
		constexpr this_type operator+(const this_type& left)const
		{
			return add(*this, left);
		}
		constexpr this_type operator-(const this_type & left)const
		{
			return subtract(*this, left);
		}
		constexpr this_type operator*(const Operational_converted& left)const
		{
			return multiply(*this, left);
		}
		constexpr this_type operator/(const Operational_converted& left)const
		{
			return divide(*this, left);
		}
		constexpr this_type & operator+=(const this_type & left)
		{
			*this = add(*this, left);
			return *this;
		}
		constexpr this_type & operator-=(const this_type & left)
		{
			*this = subtract(*this, left);
			return *this;
		}
		constexpr this_type& operator*=(const Operational_converted& left)
		{
			*this = multiply(*this, left);
			return *this;
		}
		constexpr this_type& operator/=(const Operational_converted& left)
		{
			*this = divide(*this, left);
			return *this;
		}

		constexpr std::partial_ordering operator <=>(const this_type& left)const = default;
		constexpr bool operator ==(const this_type& left)const
		{
			return value == left.value;
		}
		constexpr bool operator !=(const this_type& left)const
		{
			return value != left.value;
		}


		constexpr void set(const Minimal_reqired_float& fractional)
		{
			value = value_from_floating(fractional);
		}
		constexpr void set(const T& int_part, const T& fractional)
		{
			value = static_cast<T>(int_part * unit_value + fractional);
		}

		constexpr T int_part()const
		{
			return value / unit_value;
		}
		constexpr T fractional_part()const
		{
			return value % unit_value;
		}

		template<class Floating_number = Minimal_reqired_float>
		constexpr Floating_number as_floating()const
		{
			Floating_number result = value;
			result /= unit_value;
			return result;
		}
		constexpr const T& data()const
		{
			return value;
		}
	private:
		template<class Floating_number = Minimal_reqired_float>
		constexpr static T value_from_floating(const Floating_number& floating)
		{
			Floating_number expanded = floating * static_cast<Floating_number>(unit_value);
			return static_cast<T>(expanded);
		}

		T value = 0;
	};

}