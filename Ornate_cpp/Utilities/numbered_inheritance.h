#pragma once
#include <Types/Pack.h>

namespace uti
{
	template<class t, size_t I>
	struct inherited_on_number :t
	{
		using t::t;
	};
	template<class t_p, class = std::make_index_sequence<t_p::size>>
	struct numbered_inheritance;
	template<class ... t_v, size_t ... I_v>
	struct numbered_inheritance<typ::Pack<t_v...>, std::index_sequence<I_v...>>
		:uti::default_constructor, inherited_on_number<t_v, I_v>...
	{
		using types = typ::Pack<t_v...>;
		using default_constructor::default_constructor;
		template<class ... t_a_v>
		numbered_inheritance(t_a_v && ... providers)
			:inherited_on_number<t_v, I_v>{ std::forward<t_a_v>(providers) }...
		{}
		template<class t>
		t& get()
		{
			return static_cast<inherited_on_number<t, typ::p_index<types, t>>&>(*this);
		}
		template<class t>
		const t& get()const
		{
			return static_cast<const inherited_on_number<t, typ::p_index<types, t>>&>(*this);
		}
		template<size_t i>
		typ::p_element<i, types>& get()
		{
			return static_cast<inherited_on_number<typ::p_element<i, types>, i>&>(*this);
		}
		template<size_t i>
		const typ::p_element<i, types>& get()const
		{
			return static_cast<const inherited_on_number<typ::p_element<i, types>, i>&>(*this);
		}
	};
}