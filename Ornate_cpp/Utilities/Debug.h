#pragma once

namespace uti
{

#ifdef NDEBUG
	template<class In_debug, class In_release>
	using By_configuration = In_release;

	static constexpr bool Debug_configuration = false;
#else
	template<class In_debug, class In_release>
	using By_configuration = In_debug;

	static constexpr bool Debug_configuration = true;
#endif 
}