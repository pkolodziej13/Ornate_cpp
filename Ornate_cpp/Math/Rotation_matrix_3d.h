#pragma once
#include <algorithm>
#include "Integer_operations.h"
#include "Float_utilities.h"
#include "Direction_3d.h"
#include "Front_up.h"
#include "Axis_turn.h"

namespace mth::geo3
{
	inline mat3 orthogonalized(const mat3 & matrix);

	struct Rotation_matrix
	{
		using matrix_type = g_mat<float, 3, 3>;

		Rotation_matrix() {};
		Rotation_matrix(const matrix_type & axis_turn)
			:ortogonal(orthogonalized(axis_turn))
		{
		}
		Rotation_matrix(const Axis_turn & axis_turn)
		{
			float c = axis_turn.turn.cos();
			float s = axis_turn.turn.sin();

			const auto & axis = axis_turn.axis.vec();
			vec3 temp = axis * (1.f - c);


			ortogonal[0] = axis * temp.x + vec3(c, s* axis.z, -s * axis.y);
			ortogonal[1] = axis * temp.y + vec3(-s * axis.z, c, s *  axis.x);
			ortogonal[2] = axis * temp.z + vec3(s* axis.y, -s * axis.x, c);
		}
		Rotation_matrix(const Front_up & front_up)
			:ortogonal(front_up.right(), front_up.up(), front_up.front())
		{
		}

		Rotation_matrix & rotate(const Rotation_matrix & rotation)
		{
			ortogonal = ortogonal * rotation.matrix();
		}
		Rotation_matrix operator*(const Rotation_matrix & rotation)
		{
			return Rotation_matrix(Ortogonal_mark{}, ortogonal*rotation.matrix());
		}
		Direction operator*(const Direction & direction)
		{
			return Direction(Direction::Normalized_mark{}, ortogonal*direction.vec());
		}
		vec3 operator*(const vec3 & vec)
		{
			return ortogonal * vec;
		}

		Axis_turn axis_turn()const
		{
			struct Component_change
			{
				bool operator <(const Component_change& change)const
				{
					return dot_product < change.dot_product;
				}
				size_t dim;
				float dot_product;
			};
			std::array<Component_change, 3> changes = { {
				{0,geo3::rigth.dot(ortogonal.x)},
				{1,geo3::up.dot(ortogonal.y)},
				{2,geo3::front.dot(ortogonal.z)} } };
			std::sort(changes.begin(), changes.end());

			if (are_similar(changes[2].dot_product,1.f))
				return Axis_turn{};

			matrix_type unit_matrix{};
			vec3 greather_change = ortogonal[changes[0].dim] - unit_matrix[changes[0].dim];
			vec3 minor_change = ortogonal[changes[1].dim] - unit_matrix[changes[1].dim];

			bool non_reverted = (changes[0].dim + 1)%3 == changes[1].dim;
			Direction axis = cross(greather_change, minor_change)* positive_negative<float>(non_reverted);

			Direction initial_direction = axis.perpendicular(unit_matrix[changes[0].dim]);
			Direction final_direction = axis.perpendicular(ortogonal[changes[0].dim]);

			return { axis ,initial_direction.turn_between(final_direction) };
		}

		Front_up front_up()const
		{
			return Front_up(Direction({}, ortogonal.z), Direction({}, ortogonal.y));
		}

		const matrix_type & matrix()const
		{
			return ortogonal;
		}
		operator const matrix_type &()const
		{
			return matrix();
		}

		Rotation_matrix inversed()const
		{
			return ortogonal.transposed();
		}
		struct Ortogonal_mark {};
		Rotation_matrix(Ortogonal_mark, const matrix_type& matrix)
			:ortogonal{ matrix }
		{}

	private:

		matrix_type ortogonal;
	};

	inline bool is_non_reflected(const mat3 & matrix)
	{
		return cross(matrix.x, matrix.y).dot(matrix.z) > 0;
	}
	inline mat3 unreflect_matrix(mat3 matrix)
	{
		if (!is_non_reflected(matrix))
		{
			Direction center = matrix.x + matrix.y + matrix.z;
			matrix.x = center.reflect(matrix.x);
			matrix.y = center.reflect(matrix.y);
			matrix.z = center.reflect(matrix.z);
		}
		return matrix;
	}

	inline bool is_unit_orthogonal(const mat3& matrix, float precision = 0.001f)
	{
		return matrix.x.dot(matrix.y) < precision &&
			matrix.y.dot(matrix.z) < precision &&
			matrix.z.dot(matrix.x) < precision &&
			abs(matrix.x.powlength() - 1) < precision &&
			abs(matrix.y.powlength() - 1)< precision &&
			abs(matrix.z.powlength() - 1) < precision;
	}

	inline mat3 orthogonalized(const mat3 & orginal_matrix)
	{
		mat3 matrix = unreflect_matrix(orginal_matrix);
		if (is_unit_orthogonal(matrix))
			return matrix;

		Direction sum = matrix.x +  matrix.y + matrix.z;
		Rotation_matrix clockwise = Axis_turn{ sum,1.f / 3.f };
		Rotation_matrix counter_clockwise = clockwise.inversed();

		const float sum_part = sqrtf(1.f / 3.f);
		const float direction_part = sqrtf(2.f / 3.f);

		mat3 result;

		vec3 side_sum = matrix.x + counter_clockwise * matrix.y + clockwise * matrix.z;
		Direction leading = sum.perpendicular(side_sum);
		result.x = leading * direction_part + sum * sum_part;
		result.y = clockwise * result.x;
		result.z = counter_clockwise * result.x;

		return result;
	}

}