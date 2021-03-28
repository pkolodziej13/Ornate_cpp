#pragma once

#include <Utilities/Numbered_operations.h>
#include <Types/Pack_utility.h>
#include <Types/Function_parametrization.h>
#include <Types/cpp_utility.h>

#include "Values_computing_cell.h"

namespace uti
{
	template<class t>
	struct free_provider :provider<t>
	{
		using provider<t>::provider;
		using provider<t>::set;
	};

	namespace detail
	{
		template<class t>
		struct usage_deductor
		{
			std::tuple<> get_providers(const t& prov)
			{
				return { };
			}
		};
		template<class t>
		struct usage_deductor<provider<t>>
		{
			using providers_types = t;
			static Numeration<std::add_pointer_t<provider<t>>> get_providers(provider<t>& prov)
			{
				return Numeration<std::add_pointer_t<provider<t>>>(&prov);
			}
		};
		template<class t>
		struct usage_deductor<free_provider<t>>
		{
			using providers_types = t;
			static Numeration<std::add_pointer_t<provider<t>>> get_providers(free_provider<t>& prov)
			{
				return Numeration<std::add_pointer_t<provider<t>>>(&prov);
			}
		};
		template<class ... rec_v, class ...pro_v, class rep_i, size_t ... pro_iv>
		struct usage_deductor<computing_cell<typ::Pack<rec_v...>, typ::Pack<pro_v...>, rep_i, std::index_sequence<pro_iv...>>>
		{
			using providers_types = typ::Pack<pro_v...>;
			static Numeration<std::add_pointer_t<provider<pro_v>>...> get_providers(computing_cell<typ::Pack<rec_v...>, typ::Pack<pro_v...>>& cell)
			{
				return { &cell.get_provider<pro_iv>()... };
			}
		};

		template<class ... acesible_v>
		auto form_accesible_to_needed(const Numeration<provider<acesible_v> * ...>& num, typ::Overtype<typ::Pack<>>)
		{
			return num;
		}
		template<class ... acesible_v, class needed1, class ... needed_v>
		auto form_accesible_to_needed(const Numeration<provider<acesible_v> * ...>& num, typ::Overtype<typ::Pack< needed1, needed_v...>>)
		{
			using acesible_p = typ::Pack<acesible_v...>;
			using convertibles_p = typ::p_select<conversion_checking<needed1>::template type, acesible_p>;

			static_assert(convertibles_p::size > 0, "cannot find conversions");
			using sel = typ::p_element<0, convertibles_p>;
			constexpr size_t sel_num = typ::p_index<acesible_p, sel>;

			return add_to_numeration(num.get<sel_num>(),
				form_accesible_to_needed(remove_from_numeration<sel_num>(num), typ::Overtype<typ::Pack< needed_v...>>{}));
		}

		packalize_member(providers_types);
	}


	template<class ... parameters_v, class lam_expression>
	auto make_computing_cell(lam_expression&& func, parameters_v && ... providers_receptors_cells)
	{
		using decay_params_p = typ::p_detail< std::decay_t, typ::Pack<parameters_v...>>;
		using accesible_providers_p = typ::p_collapse < typ::p_detail<detail::packalize_providers_types, decay_params_p>>;

		std::function deducted_func{ func };
		using function_arguments = typename typ::function_prametrization<decltype(deducted_func)>::arguments_types;

		using raw_receptors_p = typ::p_select<typ::is_non_const_lvalue_refeerence, function_arguments>;
		using needed_raw_providers_p = typ::p_detach< function_arguments, raw_receptors_p>;

		auto num_num_ref = make_numeration(detail::usage_deductor<std::decay_t<parameters_v>>::get_providers(providers_receptors_cells)...);
		auto collapsed_num = n_Collapse(num_num_ref);
		using expected_providers_p = typ::p_detail<std::decay_t, needed_raw_providers_p>;

		auto destilated = detail::form_accesible_to_needed(collapsed_num, typ::Overtype<expected_providers_p>{});

		using receptors_p = typ::p_detail <typ::type_of, typ::p_detail<std::remove_pointer_t, typ::p_from<decltype(destilated)>>>;
		using providers_p = typ::p_detail<std::decay_t, raw_receptors_p>;

		using result_type = computing_cell<receptors_p, providers_p>;
		return destilated.expand([&](auto && ... destilat) { return result_type(func, *destilat...);  });

	}
}