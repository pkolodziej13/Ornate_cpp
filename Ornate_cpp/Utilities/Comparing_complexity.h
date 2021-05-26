#pragma once
#include "Computational_complexity.h"

namespace uti
{
	template<template <size_t> class Complexity >
	struct Conplexity_kind
	{
		template<size_t I>
		static void kind_trial(Complexity<I>);
	};

	template<class T1, class T2>
	struct Is_same_complexity_kind;
	template<template<size_t>class T1_t, size_t balast1, template<size_t>class T2_t, size_t balast2>
	struct Is_same_complexity_kind <T1_t<balast1>, T2_t<balast2>> :std::false_type {};
	template<template<size_t>class T_t, size_t balast1, size_t balast2>
	struct Is_same_complexity_kind<T_t<balast1>, T_t<balast2>> :std::true_type {};

	template<class T1, class T2>
	struct Faster_complexity
	{
		static constexpr size_t computing()
		{
			if constexpr (Is_same_complexity_kind<T1, T2>::value)
			{
				return T1::balast_value > T2::balast_value;
			}
			else if constexpr (typ::is_kind<T1, uti::Conplexity_kind<uti::Constant_complexity>>)
			{
				return 0;
			}
			else if constexpr (typ::is_kind<T2, uti::Conplexity_kind<uti::Constant_complexity>>)
			{
				return 1;
			}
			else if constexpr (typ::is_kind<T1, uti::Conplexity_kind<uti::Logarythmic_complexity>>)
			{
				return 0;
			}
			else if constexpr (typ::is_kind<T2, uti::Conplexity_kind<uti::Logarythmic_complexity>>)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		static constexpr size_t value = computing();
	};

	template<class Complexity1, class Complexity2>
	auto compare_complexity(Complexity1&& complexity1, Complexity2&& complexity2)
	{
		using Complexity1_type = std::decay_t<Complexity1>;
		using Complexity2_type = std::decay_t<Complexity2>;
		constexpr bool is_complexity1_constant = typ::is_kind<Complexity1_type, uti::Conplexity_kind<uti::Constant_complexity>>;
		constexpr bool is_complexity2_constant = typ::is_kind<Complexity2_type, uti::Conplexity_kind<uti::Constant_complexity>>;

		constexpr size_t if_and_probality_balst = 2;
		if constexpr (is_complexity1_constant && is_complexity2_constant)
		{
			return Comparasion_in_constexpr<  Complexity1_type::balast_value <= Complexity2_type::balast_value  >{};
		}
		else if constexpr (is_complexity1_constant)
		{
			if constexpr (Complexity1_type::balast_value <= Complexity2_type::balast_value + if_and_probality_balst)
				return Comparasion_in_constexpr< true >{};
			else
				return Comparasion_in_runtime(complexity1.total() <= complexity2.total());
		}
		else if constexpr (is_complexity2_constant)
		{
			if constexpr (Complexity2_type::balast_value <= Complexity1_type::balast_value + if_and_probality_balst)
				return Comparasion_in_constexpr< false >{};
			else
				return Comparasion_in_runtime(complexity2.total() <= complexity1.total());
		}
		else
		{
			return Comparasion_in_runtime{ complexity1.total() <= complexity2.total() };
		}
	}
}