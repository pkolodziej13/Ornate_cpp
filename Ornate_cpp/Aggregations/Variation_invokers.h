#pragma once
#include <map>

#include <Types/Pack_utility.h>

#include "Invoker.h"

namespace agg
{
	template<class Inputs_p, class Outputs_p>
	struct Variation_invokers;
	template<class ...Inputs_v, class ...Outputs_v>
	struct Variation_invokers<typ::Pack<Inputs_v...>, typ::Pack<Outputs_v...>>
	{
		using signals_p = typ::Pack<Inputs_v..., Outputs_v...>;
		using inputs_p = typ::Pack<Inputs_v...>;
		using function_type = void(Outputs_v...);

		auto add_callback_rapid(const std::function<function_type>& f, const Inputs_v &... keys)
		{
			return invokers[key_type(keys...)].add(f);
		}
		auto add_callback(const std::function<function_type>& f, const Inputs_v &... keys)
		{
			return added[key_type(keys...)].add(f);
		}
		void invoke(const Inputs_v &... keys, const Outputs_v & ... arguments)
		{
			key_type key{ keys... };
			auto search = invokers.find(key);
			if (search != invokers.end())
				search->second.invoke(arguments...);
		}
		void heartbeat()
		{
			std::vector<key_type> keys_to_delete;
			for (auto& invoker_node : invokers)
				if (invoker_node.second.empty())
					keys_to_delete.push_back(invoker_node.first);
			for (auto& key : keys_to_delete)
				invokers.erase(key);
		}
		void applay_added()
		{
			for (auto& m : added)
				invokers[m.first].steal_from(m.second);
			added.clear();
		}

	private:
		using key_type = std::tuple<Inputs_v...>;
		std::map<key_type, Invoker<Outputs_v... >> invokers;
		std::map<key_type, Invoker<Outputs_v... >> added;
	};
}