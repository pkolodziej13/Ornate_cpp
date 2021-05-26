#pragma once
#include <compare>

#include "Trigonometry.h"
#include "Arithmetic_additable.h"
#include "Arithmetic_multiplicable.h"

namespace mth
{
	struct Turn :
		mth::Arithmetic_additable<Turn, Turn>,
		mth::Arithmetic_multiplicable<Turn, float >
	{
		Turn() = default;
		Turn(float value) :value{ value } {};

		using arithmetic_additable = mth::Arithmetic_additable<Turn, Turn>;
		using arithmetic_multiplicable = mth::Arithmetic_multiplicable<Turn, float >;

		using arithmetic_additable::operator+;
		using arithmetic_additable::operator-;
		using arithmetic_additable::operator+=;
		using arithmetic_additable::operator-=;
		using arithmetic_multiplicable::operator*;
		using arithmetic_multiplicable::operator/;
		using arithmetic_multiplicable::operator*=;
		using arithmetic_multiplicable::operator/=;

		template< class Operation>
		static Turn algebraic_pair_operation(Turn const& a, Turn const& b, Operation&& operation)
		{
			return { operation(a.value,b.value) };
		}
		template<class Operation>
		static Turn algebraic_pair_operation(Turn const& a, const float& b, Operation&& operation)
		{
			return { operation(a.value,b) };
		}
		std::partial_ordering operator <=>(Turn const & right)const
		{
			return value <=> right.value;
		}
		bool operator ==(Turn const& right)const
		{
			return { value == right.value };
		}

		float radian()const
		{
			return  this->value * pi2;
		}
		float angle()const
		{
			return radian();
		}
		float sin()const
		{
			return std::sinf(radian());
		}
		float cos()const
		{
			return std::cosf(radian());
		}
		Turn operator!()
		{
			float multiplied = value * 4;
			int integer_part = int(multiplied) - (multiplied < 0);
			float reversed = 1 - multiplied + static_cast<float>(integer_part);

			return (integer_part + reversed) / 4;
		}
		inline float clockwise()const
		{
			return value;
		}
		inline void clockwise(float f)
		{
			value = f;
		}

		inline float clockwise_rest_part()const
		{
			int integer_part = int(clockwise()) - (clockwise() < 0);
			return value - float(integer_part);
		}

		static Turn from_sin(float s)
		{
			return Turn(std::asinf(s) / pi2);
		}
		static Turn from_cos(float s)
		{
			return Turn(std::acosf(s) / pi2);
		}
	private:
		float value = 0.f;
	};

	inline bool are_similar(const Turn& a, const Turn& b, float precision = float(0.01))
	{
		return abs(a.clockwise_rest_part() - b.clockwise_rest_part()) < precision
			|| abs(a.clockwise_rest_part() - (1.f - b.clockwise_rest_part())) < precision;
	}
}