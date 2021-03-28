#pragma once
#include "Used_linear_algebra.h"
#include "Rotation_matrix_3d.h"

namespace mth::geo3
{
	template<class t = float>
	struct View_matrix :g_mat<t, 4, 4>
	{
		using mat_type = g_mat<t, 4, 4>;
		using rot_mat_type = g_mat<t, 3, 3>;
		View_matrix() = default;
		View_matrix(vec3 eye, Front_up front_up)
		{
			vec3 left = front_up.right();
			this->row<0>(left);
			this->row<1>(front_up.up().vec());
			this->row<2>(-front_up.front().vec());

			this->w = vec4(-left.dot(eye), -front_up.up().dot(eye), front_up.front().dot(eye), 1);
		}
		vec3 eye()const
		{
			mat3 rotations = matrix();
			return -(rotations.transposed() *vec3(this->w));
		}
		Rotation_matrix rotation()const
		{
			mat3 rotations = matrix();
			rotations = rotations.transposed();
			rotations.z = -rotations.z;
			return Rotation_matrix(Rotation_matrix::Ortogonal_mark(), rotations);
		}
		const g_mat<t, 4, 4>& matrix()const
		{
			return *this;
		}
	};
}