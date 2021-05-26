#pragma once
#include "Numeration.h"

namespace uti
{
	template<template<class>class Representative_of, class T_p>
	struct Representatives_by_pack :typ::p_transform<Numeration, typ::p_detail<Representative_of, T_p>>
	{
		using base_type = typ::p_transform<Numeration, typ::p_detail<Representative_of, T_p>>;
		using base_type::Numeration;
		using base_type::get;
		using represented_p = T_p;
		using representing_p = typ::p_detail<Representative_of, T_p>;

		template<class T>
		Representative_of<T>& get()
		{
			static_assert(typ::p_has<T_p, T>, "called type is no represented");
			if constexpr (typ::p_has<T_p, T>)
				return this->get<typ::p_index<T_p, T>>();
		}
		template<class T>
		const Representative_of<T>& get()const
		{
			static_assert(typ::p_has<T_p, T>, "called type is no represented");
			if constexpr (typ::p_has<T_p, T>)
				return this->get<typ::p_index<T_p, T>>();
		}
		template<size_t i>
		Representative_of<typ::p_element<i, T_p>>& get()
		{
			return this->base_type::get<i>();
		}
		template<size_t i>
		const Representative_of<typ::p_element<i, T_p>>& get()const
		{
			return this->base_type::get<i>();
		}
	};
	template<template<class>class representative_of, class ... t_v>
	using Representatives = Representatives_by_pack<representative_of, typ::Pack<t_v...>>;

}