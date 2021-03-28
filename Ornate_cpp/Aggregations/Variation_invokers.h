#pragma once
#include <map>

#include <Types/Pack_utility.h>

#include "Invoker.h"

namespace agg
{
	template<class inputs_p, class outputs_p>
	struct Variation_invokers;
	template<class ...inputs_v, class ...outputs_v>
	struct Variation_invokers<typ::Pack<inputs_v...>, typ::Pack<outputs_v...>>
	{
		using signals_p = typ::Pack<inputs_v..., outputs_v...>;
		using inputs_p = typ::Pack<inputs_v...>;
		using function_type = void(outputs_v...);

		auto add_callback_rapid(const std::function<function_type>& f, const inputs_v &... keys)
		{
			return invokers[key_type(keys...)].Add(f);
		}
		auto add_callback(const std::function<function_type>& f, const inputs_v &... keys)
		{
			return added[key_type(keys...)].Add(f);
		}
		void invoke(const inputs_v &... keys, const outputs_v & ... arguments)
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
		using key_type = std::tuple<inputs_v...>;
		std::map<key_type, Invoker<outputs_v... >> invokers;
		std::map<key_type, Invoker<outputs_v... >> added;
	};
}