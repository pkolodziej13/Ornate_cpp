#pragma once
#include <functional>
#include <tuple>

#include <Types/Pack.h>

#include "numbered_inheritance.h"
#include "Values_provider_and_receptor.h"

namespace uti
{
	template<class ... t_v>
	struct multiple_receptors_inheriting
		:numbered_inheritance<typ::Pack<receptor<t_v>...>>
	{
		using types = typ::Pack<t_v...>;
		multiple_receptors_inheriting(provider<t_v> & ... providers)
			:numbered_inheritance<typ::Pack<receptor<t_v>...>>{ providers ... }
		{}
		template<class t>
		receptor<t>& get_receptor()
		{
			return this->get<receptor<t>>();
		}
		template<class t>
		const receptor<t>& get_receptor()const
		{
			return this->get<receptor<t>>();
		}
		template<size_t i>
		receptor<typ::p_element<i, types>>& get_receptor()
		{
			return this->get<i>();
		}
		template<size_t i>
		const receptor<typ::p_element<i, types>>& get_receptor()const
		{
			return this->get<i>();
		}
	};
	template<class ... t_v>
	struct multiple_providers
		:numbered_inheritance<typ::Pack<provider<t_v>...>>
	{
		using types = typ::Pack<t_v...>;
		multiple_providers() = default;

		template<class t>
		provider<t>& get_provider()
		{
			return this->get<provider<t>>();
		}
		template<class t>
		const provider<t>& get_provider()const
		{
			return this->get<provider<t>>();
		}
		template<size_t i>
		provider<typ::p_element<i, types>>& get_provider()
		{
			return this->get<i>();
		}
		template<size_t i>
		const provider<typ::p_element<i, types>>& get_provider()const
		{
			return this->get<i>();
		}
	};

	template<class receptors_p, class providers_p, class = std::make_index_sequence<receptors_p::size>, class = std::make_index_sequence<providers_p::size> >
	struct computing_cell;

	template<class ... receptors_v, class ... providers_v, size_t ... rec_iv, size_t ... pro_iv>
	struct computing_cell<typ::Pack<receptors_v...>, typ::Pack<providers_v...>, std::index_sequence<rec_iv...>, std::index_sequence<pro_iv...>>
		:multiple_receptors_inheriting<receptors_v...>, multiple_providers<providers_v...>
	{
		using base_type = multiple_receptors_inheriting<receptors_v...>;
		using providers_p = typ::Pack<providers_v...>;
		using computation_func = std::function<void(const receptors_v &..., providers_v & ...)>;
		computing_cell(computation_func func, provider<receptors_v> & ... providers)
			:base_type(providers...), func(func)
		{
			if (!func)
				throw "there must be function";
			proceed();
		}
		void proceed() override
		{
			std::tuple<providers_v ...> targets;
			auto & hidding_waring_C4101 = targets;//remove when fixed 
			func(this->get_receptor<rec_iv>().value()..., std::get<pro_iv>(targets)...);
			((this->inherited_on_number<provider<providers_v>, pro_iv>::set(std::get<pro_iv>(targets))), ...);
		}

	private:
		computation_func func;
	};

}