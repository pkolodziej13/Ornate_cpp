#pragma once
#include "Utility.h"

namespace typ
{
	template<class t>
	constexpr auto expose_type_name()
	{
		return type_of<std::decay_t<decltype(std::declval<Overtype<t>>())>>::exposing;
		using target = typename t::exposing_member;
	}
}