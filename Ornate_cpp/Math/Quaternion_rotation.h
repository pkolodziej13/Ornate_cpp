#pragma once
#include "Matrix.h"
#include "Direction_4d.h"
#include "Axis_turn.h"
#include "Complex_multiplication.h"

namespace mth::geo3
{
	template<class T>
	struct g_quat
	{
		static constexpr T zero_value = static_cast<T>(0);
		static constexpr T unit_value = static_cast<T>(1);
		static constexpr T double_value = static_cast<T>(2);
		g_quat() = default;
		g_quat(geo4::Direction<T> const& direction)
			:quaternion(direction)
		{}
		g_quat(Axis_turn const& axis_turn)
		{
			auto axis_sin = axis_turn.axis * (axis_turn.turn / double_value).sin();
			quaternion = geo4::Direction<T>(geo4::Direction<T>::Normalized_mark(),
				vec4((axis_turn.turn.radian() / 2.f).cos(), axis_sin.x, axis_sin.y, axis_sin.z));
		}
		Axis_turn axis_turn()const
		{
			return { axis() ,angle() / pi_t<T> };
		}
		Direction axis()const
		{
			T sin_part_pow = 1 - real_component() * real_component();
			if (sin_part_pow <= zero_value)
				return Direction{};
			T reverse_sin = unit_value / sqrt_temp(sin_part_pow);
			Direction direction_result{ Direction::Normalized_mark{},
				imaginary_component()* reverse_sin};
			return direction_result;
		}
		T angle()const
		{
			return std::acos(real_component()) * static_cast<T>(2);
		}

		g_vec<T, 3> rotate_vector(const g_vec<T, 3>& to_rotate)const
		{
			g_vec<T, 3> axis_sin = imaginary_component();
			g_vec<T, 3> rotation_direction = cross(axis_sin, to_rotate);
			g_vec<T, 3> correction_to_center = cross(axis_sin, rotation_direction);
			g_vec<T, 3> moving_to_middle = rotation_direction * real_component() + correction_to_center;

			return to_rotate + moving_to_middle * double_value;
		}
		g_quat<T> combine_with_quaternion(g_quat<T> const & to_combine)const
		{
			return { geo4::Direction<T>(geo4::Direction<T>::Normalized_mark(),
				complex_multiplication(quaternion.vec(),to_combine.quaternion.vec())) };
		}
		T real_component()const
		{
			return quaternion.vec().x;
		}
		g_vec<T,3> imaginary_component()const
		{
			return g_vec<T, 3>(quaternion.vec().y, quaternion.vec().z, quaternion.vec().w);
		}
	private:
		geo4::Direction<T> quaternion;
	};

}