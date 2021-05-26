#pragma once

namespace col
{
	template<class T1,class T2>
	concept Has_collision_checking = requires(T1 a, T2 b)
	{
		are_colliding(a, b);
	};

	template<class T1,class T2> requires Has_collision_checking<T1,T2>
	bool colliders_collides(const T1& a, const T2& b)
	{
		return are_colliding(a, b);
	}

	template<class T1, class T2> requires (!Has_collision_checking<T1, T2>)&& Has_collision_checking<T2, T1>
	bool colliders_collides(const T1& a, const T2& b)
	{
		return are_colliding(b, a);
	}
}