#pragma once
#include <tuple>

#include "Pack.h"

namespace typ
{
	namespace detail
	{
		template<class T, class Sequence>
		struct P_array;
		template<class T, size_t ... i_v >
		struct P_array<T, std::index_sequence<i_v...>>
		{
			template<size_t i>
			using Always_type = T;
			using type = Pack<Always_type<i_v>...>;
		};

		template<template<size_t>class Transform_template, class Sequence >
		struct P_from_index_squence;
		template<template<size_t>class Transform_template, size_t ... i_v >
		struct P_from_index_squence<Transform_template, std::index_sequence<i_v...>>
		{
			using type = Pack<Transform_template<i_v>...>;
		};

		template<class T>
		struct P_index_sequence;
		template<size_t ... i_v>
		struct P_index_sequence<std::index_sequence<i_v...>>
		{
			using type = Pack<std::integral_constant<size_t, i_v>...>;
		};
	}


	template<class Pack, auto selected_types>
	struct P_select_form_array;
	template<class ...T_v, auto array_value>
	struct P_select_form_array<Pack<T_v...>, array_value>
	{
		template<class Sequence>
		struct Expanding_indexes;
		template<size_t ... i_v>
		struct Expanding_indexes<std::index_sequence<i_v...>>
		{
			using type = Pack<typ::p_element<array_value[i_v], Pack<T_v...>>...>;
		};
		using type = typename Expanding_indexes<std::make_index_sequence<std::tuple_size<decltype(array_value)>::value>>::type;
	};

	template<class T, size_t size>
	using p_array = typename detail::P_array<T, std::make_index_sequence<size>>::type;

	template<template<size_t>class Transform_template, size_t I>
	using p_from_index_squence = typename detail::P_from_index_squence<Transform_template, std::make_index_sequence<I>>::type;

	template<size_t I>
	using p_index_sequence = typename detail::P_index_sequence<std::make_index_sequence<I>>::type;

}