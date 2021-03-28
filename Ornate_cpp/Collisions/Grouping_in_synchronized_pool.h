#pragma once
#include <Aggregations/Pool_synchronized.h>

namespace col
{
	template<class T>
	struct Grouping_in_synchronized_pool
	{
		using key_type = size_t;
		template<class Access_info>
		void add_element(Access_info& access_info)
		{
			access_info.position = elements.add(T{ access_info });
		}
		template<class Access_info>
		void remove_element(Access_info& access_info)
		{
			elements.free(access_info.position);
		}
		bool is_used()
		{
			return elements.used_size();
		}
		void synchronize()
		{
			elements.refresh();
		}
		template<class Lambda>
		void for_each(Lambda&& lambda)
		{
			for (auto& e : elements.active_range())
				lambda(e);
		}
		template<class That_segregator, class Lambda>
		void evaluate_with(That_segregator& that_segregator, Lambda&& lambda)
		{
			for_each([&](auto& collider1)
			{
				that_segregator.for_each([&](auto& collider2)
				{
					lambda(collider1, collider2);
				});
			});
		}
		template< class Lambda>
		void evaluate_self(Lambda&& lambda)
		{
			for_i(elements.total_size())
				if (elements.activity(i) == agg::Live_status::living)
					for (size_t j = i + 1; j < elements.total_size(); ++j)
						if (elements.activity(j) == agg::Live_status::living)
						{
							lambda(elements.active_or_not_at(i), elements.active_or_not_at(j));
						}
		}
		agg::Pool_synchronized<T> elements;
	};
}