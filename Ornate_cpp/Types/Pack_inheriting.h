#pragma once
#include "Pack_logic.h"

namespace typ
{
	namespace detail
	{
		template<class s, class p> struct BaseTypes;
		template<class s, class b1, class ...bb> struct BaseTypes<s, Pack<b1, bb...>>
		{
			using type = std::conditional_t<std::is_base_of_v<b1, s>, p_merge<Pack<b1>, typename BaseTypes<s, Pack<bb...>>::type>, typename BaseTypes<s, Pack<bb...>>::type>;
		};
		template<class s> struct BaseTypes<s, Pack<>>
		{
			using type = Pack<>;
		};

		template<class s, class p> struct InheritingTypes;
		template<class s, class b1, class ...bb> struct InheritingTypes<s, Pack<b1, bb...>>
		{
			using type = std::conditional_t<std::is_base_of_v< s, b1>, p_merge<Pack<b1>, typename InheritingTypes<s, Pack<bb...>>::type>, typename InheritingTypes<s, Pack<bb...>>::type>;
		};
		template<class s> struct InheritingTypes<s, Pack<>>
		{
			using type = Pack<>;
		};

		template< class t, class ... tt> struct BaseType;
		template< class t, class t1, class ... tt> struct BaseType< t, t1, tt...>
		{
			using type = typename std::conditional< std::is_same_v<t, typename InheritingTypes<t1, t>::type>, t1, typename BaseType<t, tt...>::type >::type;
		};
		template< class t1, class ... tt> struct BaseType< Pack<t1, tt...>>
		{
			using type = t1;
		};

		template<class t> struct BaseTypeOfPack;
		template<class ... tt> struct BaseTypeOfPack<Pack<tt...>>
		{
			using type = typename BaseType<Pack<tt...>, tt...>::type;
		};
	}

	template<class p>
	using baseType = typename detail::BaseTypeOfPack<p>::type;

	template<class s, class p>
	using base_types = typename detail::BaseTypes<s, p>::type;

	template<class s, class p>
	using inheriting_types = typename detail::InheritingTypes<s, p>::type;

}