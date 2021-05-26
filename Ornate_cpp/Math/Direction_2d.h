#pragma once
#include "Turn.h"
#include "Used_linear_algebra.h"
#include "Complex_multiplication.h"

namespace mth::geo2
{
	struct Direction
	{
		Direction() = default;
		Direction(vec2 direction)
			:direction(direction.normalized()) {}
		Direction(Turn clockwise)
		{
			direction.x = clockwise.sin();
			direction.y = clockwise.cos();
		}
		vec2 operator *(float scalar)const
		{
			return direction * scalar;
		}
		vec2 operator /(float scalar)const
		{
			return direction / scalar;
		}
		Direction operator-()const
		{
			return Direction(Normalized_mark{}, -direction);
		}
		vec2 complex_multiplication(const vec2 & point_to_turn)const
		{
			return mth::linear_algebra::complex_multiplication(direction, point_to_turn);
		}
		Direction turn(Turn clockvise)const
		{
			Direction complex_number(clockvise);
			return Direction(Normalized_mark{}, complex_number.complex_multiplication(direction));
		}

		inline float dot(const vec2& value)const
		{
			return direction.dot(value);
		}

		Direction right()const
		{
			return mth::cross(direction);
		}
		Direction cross()const
		{
			return mth::cross(direction);
		}
		vec2 perpendicular(const vec2& vec)const
		{
			return vec - direction * direction.dot(vec);
		}
		vec2 reflect(const vec2& vec)const
		{
			return vec + direction * dot(vec) * -2.f;
		}

		operator const vec2& ()const
		{
			return vec();
		}
		const vec2& vec()const
		{
			return direction;
		}
		Turn turn_between(const Direction& direction)const
		{
			return std::acos(this->dot(direction)) / pi2;
		}

		friend struct Rotation_matrix;
	private:
		struct Normalized_mark {};
		Direction(Normalized_mark, const vec2& direction)
			:direction(direction)
		{}
		vec2 direction = vec2(0, 1);

	};

	const Direction up{ vec2(0,1) };
	const Direction rigth{ vec2(1,0) };
	const Direction bottom{ vec2(0,-1) };
	const Direction left{ vec2(-1,0) };
}