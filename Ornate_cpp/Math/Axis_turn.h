#pragma once
#include "Direction_3d.h"

namespace mth::geo3
{
	struct Axis_turn
	{
		Axis_turn(Direction axis = Direction{}, Turn turn = 0)
			:axis(axis), turn(turn)
		{
		}

		Direction axis;
		Turn turn;
	};
}