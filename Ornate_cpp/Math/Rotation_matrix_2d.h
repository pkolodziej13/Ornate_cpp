#pragma once
#include "Used_linear_algebra.h"
#include "Direction_2d.h"

namespace mth::geo2
{
	inline mat2 orthogonalized(const mat2& matrix);

	struct Rotation_matrix
	{
		using matrix_type = g_mat<float, 2, 2>;

		Rotation_matrix() {};
		Rotation_matrix(const matrix_type& axis_turn)
			:ortogonal(orthogonalized(axis_turn))
		{}
		Rotation_matrix(const Turn& axis_turn)
			:Rotation_matrix(Direction(axis_turn))
		{}
		Rotation_matrix(const Direction& up)
			:ortogonal(up.right(),up)
		{}

		Rotation_matrix& rotate(const Rotation_matrix& rotation)
		{
			ortogonal = ortogonal * rotation.matrix();
		}
		Rotation_matrix operator*(const Rotation_matrix& rotation)
		{
			return Rotation_matrix(Ortogonal_mark{}, ortogonal * rotation.matrix());
		}
		vec2 operator*(const vec2& vec)const
		{
			return ortogonal * vec;
		}
		Turn turn()const
		{
			auto cos = ortogonal.get<0,0>();
			auto sin = ortogonal.get<1, 0>();
			return std::atan2(sin, cos) / pi2;
		}

		const matrix_type& matrix()const
		{
			return ortogonal;
		}
		operator const matrix_type& ()const
		{
			return matrix();
		}

		Rotation_matrix inversed()const
		{
			return ortogonal.transposed();
		}

	private:
		struct Ortogonal_mark {};
		Rotation_matrix(Ortogonal_mark, const matrix_type& matrix)
			:ortogonal{ matrix }
		{}

		matrix_type ortogonal;
	};

	inline bool is_non_reflected(const mat2& matrix)
	{
		return cross(matrix.y).dot(matrix.x)> 0;
	}
	inline mat2 unreflect_matrix(mat2 matrix)
	{
		if (!is_non_reflected(matrix))
		{
			Direction center = matrix.x + matrix.y;
			matrix.x = center.reflect(matrix.x);
			matrix.y = center.reflect(matrix.y);
		}
		return matrix;
	}

	inline bool is_unit_orthogonal(const mat2& matrix, float precision = 0.001f)
	{
		return matrix.x.dot(matrix.y) < precision &&
			abs(matrix.x.powlength() - 1) < precision &&
			abs(matrix.y.powlength() - 1) < precision;
	}

	inline mat2 orthogonalized(const mat2& orginal_matrix)
	{
		mat2 matrix = unreflect_matrix(orginal_matrix);
		if (is_unit_orthogonal(matrix))
			return matrix;

		Direction leading_direction = matrix.x +  matrix.y;
		Direction x = leading_direction.turn(1.f/8.f);
		Direction y = -x.right();
		return { x ,y };
	}

}