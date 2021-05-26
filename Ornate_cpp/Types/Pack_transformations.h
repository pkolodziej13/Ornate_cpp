#pragma once
#include "Pack_logic.h"

namespace typ
{
	namespace detail
	{
		template<class T>
		struct P_from;
		template<template<class ...>class Template_type, class ... T_v>
		struct P_from<Template_type<T_v...>>
		{
			using type = Pack<T_v...>;
		};

		template<template<typename > class Modificator, class T>
		struct P_detail;
		template<template<typename> class  Modificator, class ...T_v >
		struct P_detail< Modificator, Pack<T_v...>  >
		{
			using type = Pack<Modificator<T_v>...>;
		};

		template<template<typename> class  Template_type, class T>
		struct P_select;
		template<template<typename> class  Template_type, class T, class ...T_v>
		struct P_select<Template_type, Pack<T, T_v...>>
		{
			using pom = typename P_select<Template_type, Pack<T_v...>>::type;
			using type = std::conditional_t<Template_type<T>::value
				, p_merge<Pack<T>, pom>
				, pom>;
		};
		template<template<typename> class  Template_type> 
		struct P_select<Template_type, Pack<>>
		{
			using type = Pack<>;
		};

		template<template<typename ... >class Template_type, class T>
		struct P_transfrom;
		template<template<typename ... >class Template_type, class ...T_v>
		struct P_transfrom<Template_type, Pack<T_v...>>
		{
			using type = Template_type<T_v...>;
		};
	}

	template<class T>
	using p_from = typename detail::P_from<T>::type;

	template<template<typename> class  Template_type, class T>
	using p_detail = typename detail::P_detail<Template_type, T>::type;

	template<template<typename> class  Template_type, class P>
	using p_select = typename detail::P_select<Template_type, P>::type;
	template< class P, class Selector>
	using p_select_raw = typename detail::P_select<typename Selector::template selector, P>::type;

	template<template<typename ... >class Template_type, class P>
	using p_transform = typename detail::P_transfrom<Template_type, P>::type;
}