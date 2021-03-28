#pragma once
#include <tuple>

#include "Pack.h"

namespace typ
{
	namespace detail
	{
		template<class t, class sequence>
		struct P_array;
		template<class t, size_t ... I >
		struct P_array<t, std::index_sequence<I...>>
		{
			template<size_t i>
			using i_type = t;
			using type = Pack<i_type<I>...>;
		};

		template<template<size_t>class transform, class sequence >
		struct P_from_index_squence;
		template<template<size_t>class transform, size_t ... sequence >
		struct P_from_index_squence<transform, std::index_sequence<sequence...>>
		{
			using type = Pack<transform<sequence>...>;
		};

		template<class t>
		struct P_index_sequence;
		template<size_t ... i_v>
		struct P_index_sequence<std::index_sequence<i_v...>>
		{
			using type = Pack<std::integral_constant<size_t, i_v>...>;
		};
	}


	template<class Pack, auto selected_types>
	struct P_select_form_array;
	template<class ...pp, auto aa>
	struct P_select_form_array<Pack<pp...>, aa>
	{
		template<class Sequence>
		struct Expanding_indexes;
		template<size_t ... I>
		struct Expanding_indexes<std::index_sequence<I...>>
		{
			using type = Pack<typ::p_element<aa[I], Pack<pp...>>...>;
		};
		using type = typename Expanding_indexes<std::make_index_sequence<std::tuple_size<decltype(aa)>::value>>::type;
	};

	template<class t, size_t size>
	using p_array = typename detail::P_array<t, std::make_index_sequence<size>>::type;

	template<template<size_t>class transform, size_t I>
	using p_from_index_squence = typename detail::P_from_index_squence<transform, std::make_index_sequence<I>>::type;

	template<size_t I>
	using p_index_sequence = typename detail::P_index_sequence<std::make_index_sequence<I>>::type;

}