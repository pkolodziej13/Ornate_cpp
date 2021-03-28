#pragma once
#include "Used_linear_algebra.h"

namespace mth::geo4
{
	template<class T>
	struct Direction
	{
		Direction() = default;
		Direction(g_vec<T, 4> const& direction)
			:direction(direction.normalized()) {}
		struct Normalized_mark {};
		Direction(Normalized_mark, g_vec<T, 4> const& direction)
			:direction(direction)
		{}
		g_vec<T, 4> const& vec()const
		{
			return direction;
		}
	private:
		g_vec<T, 4> const& direction{ 0, 0, 1,0 };

	};
}