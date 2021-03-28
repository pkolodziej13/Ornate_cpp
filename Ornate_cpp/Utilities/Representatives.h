#pragma once
#include "Numeration.h"

namespace uti
{
	template<template<class>class representative_of, class t_p>
	struct Representatives_by_pack :typ::p_transform<Numeration, typ::p_detail<representative_of, t_p>>
	{
		using base_type = typ::p_transform<Numeration, typ::p_detail<representative_of, t_p>>;
		using base_type::Numeration;
		using base_type::get;
		using represented_p = t_p;
		using representing_p = typ::p_detail<representative_of, t_p>;

		template<class t>
		representative_of<t>& get()
		{
			static_assert(typ::p_has<t_p, t>, "called type is no represented");
			if constexpr (typ::p_has<t_p, t>)
				return this->get<typ::p_index<t_p, t>>();
		}
		template<class t>
		const representative_of<t>& get()const
		{
			static_assert(typ::p_has<t_p, t>, "called type is no represented");
			if constexpr (typ::p_has<t_p, t>)
				return this->get<typ::p_index<t_p, t>>();
		}
		template<size_t i>
		representative_of<typ::p_element<i, t_p>>& get()
		{
			return this->base_type::get<i>();
		}
		template<size_t i>
		const representative_of<typ::p_element<i, t_p>>& get()const
		{
			return this->base_type::get<i>();
		}
	};
	template<template<class>class representative_of, class ... t_v>
	using Representatives = Representatives_by_pack<representative_of, typ::Pack<t_v...>>;

}