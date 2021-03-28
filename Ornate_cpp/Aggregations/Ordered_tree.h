#pragma once
#include <functional>
#include <cassert>

#include <Utilities/Loops.h>
#include <Utilities/Range_for_loop.h>

namespace agg
{
	template<class T>
	struct Ordered_tree
	{
		Ordered_tree& add_child()
		{
			childs.push_back(Ordered_tree<T>());
			return childs.back();
		}
		std::vector<std::reference_wrapper<T>> all_childs_objects()
		{
			std::vector<std::reference_wrapper<T>> result;
			result.push_back(std::reference_wrapper<T>(object));
			for (auto& l : childs)
			{
				auto l_chil = l.all_childs_objects();
				result.insert(result.end(), l_chil.begin(), l_chil.end());
			}
			return result;
		}
		const std::list<Ordered_tree<T>>& get_childs()const
		{
			return childs;
		}
		T* operator->()
		{ 
			return &object; 
		}
		const T* operator->()const
		{
			return &object;
		}
		T& get()
		{
			return object;
		}
		const T& get()const
		{
			return object;
		}

		struct Loop_range
		{
			static constexpr auto features =
				uti::Loop_features::aggregator_unintacted|
				uti::Loop_features::order_unintacted;

			Loop_range(Ordered_tree<T>& it)
			{
				node = &it;
			}
			bool finished()const
			{
				return node == 0;
			}
			T& current_element()const
			{
				return node->get();
			}
			void increment()
			{
				assert(("iterator is ended", !finished()));

				if (!node->childs.empty())
				{
					if (!lists.empty())
					{
						lists.back().first++;
					}
					lists.push_back({ node->childs.begin(),node->childs.end() });
					node = &(*lists.back().first);
				}
				else
				{
					if (!lists.empty())
					{
						lists.back().first++;
					}
					while (!lists.empty() && lists.back().first == lists.back().second)
						lists.pop_back();
					if (!lists.empty())
					{
						node = &(*lists.back().first);
					}
					if (lists.empty())
					{
						node = 0;
					}

				}
			}
		private:
			Ordered_tree<T>* node = 0;
			using list_iterator = typename std::list<Ordered_tree<T>>::iterator;
			list_iterator last_end;
			std::vector<std::pair< list_iterator, list_iterator>> lists;
		};
		auto range()
		{
			return uti::Range_for_loop < Loop_range>(*this);
		}

	private:
		std::list<Ordered_tree<T>> childs;
		T object;
	};
}