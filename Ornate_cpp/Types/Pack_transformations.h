#pragma once
#include "Pack_logic.h"

namespace typ
{
	namespace detail
	{
		template<class n>
		struct P_from;
		template<template<class ...>class temp, class ... tt>
		struct P_from<temp<tt...>>
		{
			using type = Pack<tt...>;
		};

		template<template<typename > class modificator, class t>
		struct P_detail;
		template<template<typename> class  modificator, class ...T_v >
		struct P_detail< modificator, Pack<T_v...>  >
		{
			using type = Pack<modificator<T_v>...>;
		};

		template<template<typename> class  c, class t>
		struct P_select;
		template<template<typename> class  c, class p1, class ...pp>
		struct P_select<c, Pack<p1, pp...>>
		{
			using pom = typename P_select<c, Pack<pp...>>::type;
			using type = std::conditional_t<c<p1>::value
				, p_merge<Pack<p1>, pom>
				, pom>;
		};
		template<template<typename> class  c> struct P_select<c, Pack<>>
		{
			using type = Pack<>;
		};

		template<template<typename ... >class t, class p>
		struct P_transfrom;
		template<template<typename ... >class t, class ...pp>
		struct P_transfrom<t, Pack<pp...>>
		{
			using type = t<pp...>;
		};
	}

	template<class t>
	using p_from = typename detail::P_from<t>::type;

	template<template<typename> class  _t, class t>
	using p_detail = typename detail::P_detail<_t, t>::type;

	template<template<typename> class  c, class p>
	using p_select = typename detail::P_select<c, p>::type;
	template< class pack, class selector>
	using p_select_raw = typename detail::P_select<typename selector::template selector, pack>::type;

	template<template<typename ... >class t, class p>
	using p_transform = typename detail::P_transfrom<t, p>::type;
}