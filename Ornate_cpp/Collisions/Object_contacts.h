#pragma once

#include <Utilities/Representatives.h>
#include <Aggregations/vector_set.h>


namespace col
{
	template<class T>
	struct Differences_detector
	{
		template<class On_continue, class On_add, class On_remove>
		void new_ones_to_current(On_add&& on_add, On_continue&& on_continue, On_remove&& on_remove)
		{
			if (current.size() == 0)
			{
				for_i(new_ones.size())
					on_add(new_ones[i]);
			}
			else if (new_ones.size() == 0)
			{
				for_i(current.size())
					on_remove(current[i]);
			}
			else
			{
				size_t current_i = 0;
				size_t new_i = 0;
				while (true)
				{
					if (current[current_i] == new_ones[new_i])
					{
						on_continue(current[current_i]);
						++current_i;
						++new_i;
					}
					else if (current[current_i] < new_ones[new_i])
					{
						on_remove(current[current_i]);
						++current_i;
						if (current_i == current.size())
						{
							for (; new_i < new_ones.size(); ++new_i)
								on_add(new_ones[new_i]);
							break;
						}
					}
					else if (current[current_i] > new_ones[new_i])
					{
						on_add(new_ones[new_i]);
						++new_i;
						if (new_i == new_ones.size())
						{
							for (; current_i < current.size(); ++current_i)
								on_remove(current[current_i]);
							break;
						}
					}
				}
			}
			move_new_to_current();
		}
		template<class On_continue, class On_add, class On_remove>
		void new_ones_to_current_remove_safe(On_add&& on_add, On_continue&& on_continue, On_remove&& on_remove)
		{
			auto merged = new_ones.merge(current);
			for_i(merged.size())
			{
				bool in_new = new_ones.contains(merged[i]);
				bool in_current = current.contains(merged[i]);
				if (in_current && in_new)
				{
					on_continue(merged[i]);
				}
				else if (in_current)
				{
					current.remove(merged[i]);
					on_remove(merged[i]);
				}
				else if (in_new)
				{
					current.add(merged[i]);
					on_add(merged[i]);
				}
			}
			new_ones.clear();
		}
		void collect(T to_collect)
		{
			new_ones.add(to_collect);
		}
		void move_new_to_current()
		{
			current = new_ones;
			new_ones.clear();
		}
		void clear_new()
		{
			new_ones.clear();
		}
		void erase(T to_erase)
		{
			current.remove(to_erase);
			new_ones.remove(to_erase);
		}
		void add_current(T to_add)
		{
			current.add(to_add);
		}

		agg::vector_set<T> current;
		agg::vector_set<T> new_ones;
	};

	template<class >
	using Contact_type = Differences_detector<size_t>;

	template<class Contacts_types_p>
	using Contacts_representatives = uti::Representatives_by_pack<Contact_type, Contacts_types_p>;

	template<class Object_type,class Comparer>
	using Object_contacts = Contacts_representatives<typename Comparer::template Colliding_objects<Object_type>>;

}