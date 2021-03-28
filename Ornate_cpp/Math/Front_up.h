#pragma once
#include "Direction_3d.h"

namespace mth::geo3
{
	struct Front_up
	{
		Front_up() {};
		Front_up(Direction front, vec3 up = geo3::up)
			:front_hermetized(front), up_hermetized(cross<float>(front.cross(up), front))
		{}
		const Direction& front()const
		{
			return front_hermetized;
		}
		const Direction& up()const
		{
			return up_hermetized;
		}
		Direction right()const
		{
			return Direction(Direction::Normalized_mark{}, up_hermetized.cross(front_hermetized));
		}
		Direction left()const
		{
			return Direction(Direction::Normalized_mark{}, front_hermetized.cross(up_hermetized));
		}
	private:
		Direction front_hermetized = geo3::front;
		Direction up_hermetized = geo3::up;
	};
}