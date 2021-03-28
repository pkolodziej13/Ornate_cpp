#pragma once
#include "Turn.h"
#include "Used_linear_algebra.h"
#include "Complex_multiplication.h"

namespace mth::geo3
{
	struct Direction
	{
		Direction() = default;
		Direction(vec3 direction)
			:direction(direction.normalized()) {}
		Direction(Turn up_front, Turn front_right, Turn right_up)
		{
			vec2 yz(up_front.cos(), up_front.sin());
			vec2 zx = linear_algebra::complex_multiplication(vec2(yz.y,0), vec2(front_right.cos(), front_right.sin()));
			vec2 xy = complex_multiplication(vec2(zx.y, yz.x), vec2(right_up.cos(), right_up.sin()));
			direction = vec3(xy.x, xy.y, zx.x);
		}
		struct Normalized_mark {};
		Direction(Normalized_mark, const vec3& direction)
			:direction(direction)
		{}
		vec3 operator *(float v)const
		{
			return direction * v;
		}
		vec3 operator /(float v)const
		{
			return direction / v;
		}
		Direction operator-()const
		{
			return Direction(Normalized_mark{}, -direction);
		}

		inline float dot(const vec3& value)const
		{
			return direction.dot(value);
		}
		vec3 cross(const vec3& value)const
		{
			return mth::cross(direction, value);
		}
		vec3 perpendicular(const vec3& vec)const
		{
			return vec - direction * direction.dot(vec);
		}
		vec3 reflect(const vec3& vec)const
		{
			return vec + direction * dot(vec) * -2.f;
		}

		operator const vec3& ()const
		{
			return vec();
		}
		const vec3& vec()const
		{
			return direction;
		}
		Turn turn_between(const Direction& direction)const
		{
			return std::acos(this->dot(direction)) / pi2;
		}
	private:
		vec3 direction = vec3(0, 1, 0);

	};

	const Direction up{ vec3(0,1,0) };
	const Direction rigth{ vec3(1,0,0) };
	const Direction front{ vec3(0,0,1) };
	const Direction bottom{ vec3(0,-1,0) };
	const Direction left{ vec3(-1,0,0) };
	const Direction back{ vec3(0,0,-1) };
}

