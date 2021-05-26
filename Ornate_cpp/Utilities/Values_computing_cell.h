#pragma once
#include <functional>
#include <tuple>
#include <stdexcept>

#include <Types/Pack.h>

#include "Numbered_inheritance.h"
#include "Values_provider_and_receptor.h"

namespace uti
{
	template<class ... T_v>
	struct Multiple_receptors_inheriting
		:Numbered_inheritance<typ::Pack<Receptor<T_v>...>>
	{
		using Types_p = typ::Pack<T_v...>;
		Multiple_receptors_inheriting(Provider<T_v> & ... providers)
			:Numbered_inheritance<typ::Pack<Receptor<T_v>...>>{ providers ... }
		{}
		template<class T>
		Receptor<T>& get_receptor()
		{
			return this->get<Receptor<T>>();
		}
		template<class T>
		const Receptor<T>& get_receptor()const
		{
			return this->get<Receptor<T>>();
		}
		template<size_t i>
		Receptor<typ::p_element<i, Types_p>>& get_receptor()
		{
			return this->get<i>();
		}
		template<size_t i>
		const Receptor<typ::p_element<i, Types_p>>& get_receptor()const
		{
			return this->get<i>();
		}
	};
	template<class ... T_v>
	struct Multiple_providers
		:Numbered_inheritance<typ::Pack<Provider<T_v>...>>
	{
		using Types_p = typ::Pack<T_v...>;
		Multiple_providers() = default;

		template<class T>
		Provider<T>& get_provider()
		{
			return this->get<Provider<T>>();
		}
		template<class T>
		const Provider<T>& get_provider()const
		{
			return this->get<Provider<T>>();
		}
		template<size_t i>
		Provider<typ::p_element<i, Types_p>>& get_provider()
		{
			return this->get<i>();
		}
		template<size_t i>
		const Provider<typ::p_element<i, Types_p>>& get_provider()const
		{
			return this->get<i>();
		}
	};

	template<class Receptors_p, class Providers_p, class = std::make_index_sequence<Receptors_p::size>, class = std::make_index_sequence<Providers_p::size> >
	struct Computing_cell;

	template<class ... Receptors_v, class ... Providers_v, size_t ... rec_iv, size_t ... pro_iv>
	struct Computing_cell<typ::Pack<Receptors_v...>, typ::Pack<Providers_v...>, std::index_sequence<rec_iv...>, std::index_sequence<pro_iv...>>
		:Multiple_receptors_inheriting<Receptors_v...>, Multiple_providers<Providers_v...>
	{
		using base_type = Multiple_receptors_inheriting<Receptors_v...>;
		using Providers_p = typ::Pack<Providers_v...>;
		using Computation_func = std::function<void(const Receptors_v &..., Providers_v & ...)>;
		Computing_cell(Computation_func function_to_set, Provider<Receptors_v> & ... providers)
			:base_type(providers...), function(function_to_set)
		{
			if (!function)
				throw std::runtime_error("there must be a function");
			proceed();
		}
		void proceed() override
		{
			std::tuple<Providers_v ...> targets;
			auto& hiding_warning_C4101 = targets;//remove on VS fix
			function(this->get_receptor<rec_iv>().value()..., std::get<pro_iv>(targets)...);
			((this->Inherited_on_number<Provider<Providers_v>, pro_iv>::set(std::get<pro_iv>(targets))), ...);
		}

	private:
		Computation_func function;
	};

}