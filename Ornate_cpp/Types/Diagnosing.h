#pragma once
#include "Utility.h"

namespace typ
{
	template<class T>
	constexpr auto expose_type_name()
	{
		return type_of<std::decay_t<decltype(std::declval<Overtype<T>>())>>::exposing;
		using target = typename T::exposing_member;
	}
}