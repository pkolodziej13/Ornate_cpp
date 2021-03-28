#pragma once
#include <functional>
#include <Utilities/Non_replicable.h>

#include "Loose_List.h"

namespace agg
{
	template<class ... args>
	struct Invoker :uti::non_replicable
	{
		using this_type = Invoker<args...>;
		Loose_element_shared<std::function<void(args...)>> Add(const std::function<void(args...)>& function)
		{
			return to_invoke.Create_as_last(function);
		}
		void invoke(args ... arguments)
		{
			for (auto& ti : to_invoke.Save_range())
			{
				ti(arguments...);
			}
		}
		bool empty()
		{
			return to_invoke.single();
		}
		void steal_from(this_type& to_steal)
		{
			to_invoke.Link_chain_as_previous(to_steal.to_invoke);
			to_steal.to_invoke.detach();
		}
	private:
		Loose_list<std::function<void(args...)>> to_invoke;
	};

}