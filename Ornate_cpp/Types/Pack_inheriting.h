#pragma once
#include "Pack_logic.h"

namespace typ
{
	namespace detail
	{
		template<class Derived, class Potential_base_types_p>
		struct P_base_types;
		template<class Derived, class T, class ...T_v>
		struct P_base_types<Derived, Pack<T, T_v...>>
		{
			using Inner_pack = typename P_base_types<Derived, Pack<T_v...>>::type;
			using type = std::conditional_t<std::is_base_of_v<T, Derived>, p_merge<Pack<T>, Inner_pack>, Inner_pack>;
		};
		template<class Derived >
		struct P_base_types<Derived, Pack<>>
		{
			using type = Pack<>;
		};

		template<class Base_type, class Potential_derived_types_p>
		struct P_inheriting_types;
		template<class Base_type, class T, class ...T_v>
		struct P_inheriting_types<Base_type, Pack<T, T_v...>>
		{
			using Inner_pack = typename P_inheriting_types<Base_type, Pack<T_v...>>::type > ;
			using type = std::conditional_t<std::is_base_of_v< Base_type, T>, p_merge<Pack<T>, Inner_pack>, Inner_pack>;
		};
		template<class Base_type>
		struct P_inheriting_types<Base_type, Pack<>>
		{
			using type = Pack<>;
		};
	}

	template<class Derived, class Potential_base_types_p>
	using p_base_types = typename detail::P_base_types<Derived, Potential_base_types_p>::type;

	template<class Base_type, class Potential_derived_types_p>
	using p_inheriting_types = typename detail::P_inheriting_types<Base_type, Potential_derived_types_p>::type;

}