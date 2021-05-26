#pragma once
#include <Types/Pack.h>
#include "Utility.h"

namespace uti
{
	template<class T, size_t I>
	struct Inherited_on_number :T
	{
		using T::T;
	};
	template<class T_p, class = std::make_index_sequence<T_p::size>>
	struct Numbered_inheritance;
	template<class ... T_v, size_t ... I_v>
	struct Numbered_inheritance<typ::Pack<T_v...>, std::index_sequence<I_v...>>
		:uti::default_constructor, Inherited_on_number<T_v, I_v>...
	{
		using Types_p = typ::Pack<T_v...>;
		using default_constructor::default_constructor;
		template<class ... Arguments_types_v>
		Numbered_inheritance(Arguments_types_v && ... arguments_v)
			:Inherited_on_number<T_v, I_v>{ std::forward<Arguments_types_v>(arguments_v) }...
		{}
		template<class T>
		T& get()
		{
			return static_cast<Inherited_on_number<T, typ::p_index<Types_p, T>>&>(*this);
		}
		template<class T>
		const T& get()const
		{
			return static_cast<const Inherited_on_number<T, typ::p_index<Types_p, T>>&>(*this);
		}
		template<size_t i>
		typ::p_element<i, Types_p>& get()
		{
			return static_cast<Inherited_on_number<typ::p_element<i, Types_p>, i>&>(*this);
		}
		template<size_t i>
		const typ::p_element<i, Types_p>& get()const
		{
			return static_cast<const Inherited_on_number<typ::p_element<i, Types_p>, i>&>(*this);
		}
	};
}