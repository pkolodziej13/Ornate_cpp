#pragma once
#include "Utility.h"

namespace typ
{
	struct Information
	{
		constexpr Information()
		{
		}
		template<class t>
		constexpr Information(Overtype<t>)
			:size(sizeof(t))
		{
		}
		size_t size{};
	};
}