#pragma once
#include "Variation_invokers.h"

namespace agg
{
	template<class To_add>
	struct p_push_fron_type 
	{
		template<class p>
		using type = typ::p_add<To_add, p>;
	};
	template<class Types_p>
	struct Variation_maker;
	template<class T, class ... Types_v>
	struct Variation_maker<typ::Pack<T, Types_v...>>
	{
		using variations_without_first = typename Variation_maker<typ::Pack<Types_v...>>::type;
		using variations_with_first = typ::p_detail<typename p_push_fron_type<T>::template type, variations_without_first>;
		using type = typ::p_add< variations_with_first, variations_without_first >;
	};
	template<>
	struct Variation_maker<typ::Pack<>>
	{
		using type = typ::Pack<typ::Pack<>>;
	};

	template<class ... To_inheritance_v>
	struct Variations_callbacks_user;
	template< class To_inheritance, class ... To_inheritance_v>
	struct Variations_callbacks_user< To_inheritance, To_inheritance_v...>
		:To_inheritance, Variations_callbacks_user<To_inheritance_v...>
	{
		using To_inheritance::add_callback;
		using Variations_callbacks_user<To_inheritance_v...>::add_callback;
		using To_inheritance::add_callback_rapid;
		using Variations_callbacks_user<To_inheritance_v...>::add_callback_rapid;
	};
	template<class To_inheritance>
	struct Variations_callbacks_user<To_inheritance> :To_inheritance
	{
		using To_inheritance::add_callback;
		using To_inheritance::add_callback_rapid;
	};
	template<>
	struct Variations_callbacks_user<>
	{
	};

	template<class Signals_p, class ... To_inheritance_v >
	struct Variations_callbacks_user_inheritance;
	template<class ... Signals_v, class ... To_inheritance_v >
	struct Variations_callbacks_user_inheritance<typ::Pack<Signals_v...>, To_inheritance_v...>
		:Variations_callbacks_user<To_inheritance_v...>
	{
		using base_types_p = typ::Pack<To_inheritance_v...>;
		using signals_p = typ::Pack<Signals_v...>;

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
		inline void invoke(Signals_v ... signals_a)
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
			(To_inheritance_v::heartbeat(), ...);
		}

		void applay_added()
		{
			(static_cast<To_inheritance_v*>(this)->applay_added(), ...);
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

	template<class Inputs_p, class Variants_inputs_p, class Variants_p, class Outputs_p>
	struct Variations_callbacks_apportionment;
	template<class Inputs_p, class ... Variants_inputs_v, class Variants_p, class Outputs_p>
	struct Variations_callbacks_apportionment< Inputs_p, typ::Pack<Variants_inputs_v...>, Variants_p, Outputs_p>
		:Variations_callbacks_user_inheritance< typ::p_add<typ::p_add<Inputs_p, Variants_p>, Outputs_p>,
		Variation_invokers<typ::p_add<Inputs_p, Variants_inputs_v>, typ::p_add< typ::p_detach<Variants_p, Variants_inputs_v>, Outputs_p>>...>
	{
	};
	template<class Inputs_p, class Variations_p, class Outputs_p>
	struct Variations_callbacks
		:Variations_callbacks_apportionment<Inputs_p, typ::type_of<Variation_maker<Variations_p>>, Variations_p, Outputs_p>
	{
	};
}