#pragma once
#include "Variation_invokers.h"

namespace agg
{
	template<class to_add>
	struct p_push_fron_type {
		template<class p>
		using type = typ::p_add<to_add, p>;
	};
	template<class types_P>
	struct Variation_maker;
	template<class t1, class ... types_v>
	struct Variation_maker<typ::Pack<t1, types_v...>>
	{
		using variations_without_t1 = typename Variation_maker<typ::Pack<types_v...>>::type;
		using variations_with_t1 = typ::p_detail<typename p_push_fron_type<t1>::template type, variations_without_t1>;
		using type = typ::p_add< variations_with_t1, variations_without_t1 >;
	};
	template<>
	struct Variation_maker<typ::Pack<>>
	{
		using type = typ::Pack<typ::Pack<>>;
	};

	template<class ... to_inheritance_v>
	struct Variations_callbacks_user;
	template< class to_inheritance1, class ... to_inheritance_v>
	struct Variations_callbacks_user< to_inheritance1, to_inheritance_v...>
		:to_inheritance1, Variations_callbacks_user<to_inheritance_v...>
	{
		using to_inheritance1::add_callback;
		using Variations_callbacks_user<to_inheritance_v...>::add_callback;
		using to_inheritance1::add_callback_rapid;
		using Variations_callbacks_user<to_inheritance_v...>::add_callback_rapid;
	};
	template<class to_inheritance>
	struct Variations_callbacks_user<to_inheritance> :to_inheritance
	{
		using to_inheritance::add_callback;
		using to_inheritance::add_callback_rapid;
	};
	template<>
	struct Variations_callbacks_user<>
	{
	};

	template<class signals_p, class ... to_inheritance_v >
	struct Variations_callbacks_user_inheritance;
	template<class ... signals_v, class ... to_inheritance_v >
	struct Variations_callbacks_user_inheritance<typ::Pack<signals_v...>, to_inheritance_v...>
		:Variations_callbacks_user<to_inheritance_v...>
	{
		using base_types_p = typ::Pack<to_inheritance_v...>;
		using signals_p = typ::Pack<signals_v...>;

		template<class invoded_varation, class  tie_type>
		inline void call_invoke(tie_type& arg_tie)
		{
			using type_signals_p = typename invoded_varation::signals_p;
			invoded_varation& variation = *this;
			type_signals_p::expand([&](auto ... s)
			{
				variation.invoke(std::get<typ::p_index<signals_p, typ::type_of<decltype(s)>>>(arg_tie)...);
			}
			);
		}
		inline void invoke(signals_v ... signals_a)
		{
			auto arg_tie = std::tie(signals_a...);
			base_types_p::for_each([&](auto t)
			{
				using type = typ::type_of<decltype(t)>;
				call_invoke<type>(arg_tie);
			});
		}
		void heartbeat()
		{
			(to_inheritance_v::heartbeat(), ...);
		}

		void applay_added()
		{
			(static_cast<to_inheritance_v*>(this)->applay_added(), ...);
		}
		template<class p>
		struct checking
		{
			template<class t>
			using type = typ::detail::P_matches<p, typename t::inputs_p>;
		};
		template<class ... inputs_v>
		static constexpr bool has_matched_variation()
		{
			using inputs_p_a = typ::Pack<inputs_v...>;
			using selected_p = typ::p_select<typename checking<inputs_p_a>::template type, base_types_p>;
			return selected_p::size > 0;
		}
		template<class ... inputs_v>
		using matching_variation = typ::p_element < 0, typ::p_select<typename checking<typ::Pack<inputs_v...>>::template type, base_types_p>>;
	};

	template<class inputs_p, class variants_inputs_p_p, class variants_p, class outputs_p>
	struct Variations_callbacks_apportionment;
	template<class inputs_p, class ... variants_inputs_p_v, class variants_p, class outputs_p>
	struct Variations_callbacks_apportionment< inputs_p, typ::Pack<variants_inputs_p_v...>, variants_p, outputs_p>
		:Variations_callbacks_user_inheritance< typ::p_add<typ::p_add<inputs_p, variants_p>, outputs_p>,
		Variation_invokers<typ::p_add<inputs_p, variants_inputs_p_v>, typ::p_add< typ::p_detach<variants_p, variants_inputs_p_v>, outputs_p>>...>
	{
	};
	template<class inputs_p, class variations_p, class outputs_p>
	struct Variations_callbacks
		:Variations_callbacks_apportionment<inputs_p, typ::type_of<Variation_maker<variations_p>>, variations_p, outputs_p>
	{
	};
}