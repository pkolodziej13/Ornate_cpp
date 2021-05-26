#pragma once
#include <functional>
#include <Utilities/Non_replicable.h>

#include "Loose_List.h"

namespace agg
{
	template<class ... Args_v>
	struct Invoker :uti::non_replicable
	{
		using this_type = Invoker<Args_v...>;
		Loose_element_shared<std::function<void(Args_v...)>> add(const std::function<void(Args_v...)>& function)
		{
			return to_invoke.create_as_last(function);
		}
		void invoke(Args_v ... arguments)
		{
			for (auto& function : to_invoke.save_range())
			{
				function(arguments...);
			}
		}
		bool empty()
		{
			return to_invoke.single();
		}
		void steal_from(this_type& to_steal)
		{
			to_invoke.link_chain_as_previous(to_steal.to_invoke);
			to_steal.to_invoke.detach();
		}
	private:
		Loose_list<std::function<void(Args_v...)>> to_invoke;
	};

}