#pragma once
#include <functional>

namespace uti
{
	template<class Range_type>
	struct Range_for_loop
	{
		struct Iterator_handler
		{
			Iterator_handler(Range_type& range)
				:range{ range } {}
			void operator++()
			{
				range.increment();
			}
			using element_type = decltype(std::declval<Range_type>().current_element());
			inline element_type operator*()
			{
				return range.current_element();
			}
			inline bool operator !=(const Iterator_handler& b)
			{
				bool bb  = !range.finished();
				return !range.finished();
			}
		private:
			Range_type& range;
		};

		template<class ... Args_v>
		Range_for_loop(Args_v && ... args_v)
			:range(std::forward<Args_v>(args_v)...)
		{}
		Iterator_handler begin()
		{
			return { range };
		}
		Iterator_handler end()
		{
			return { range };
		}

	private:
		Range_type range;
	};

}