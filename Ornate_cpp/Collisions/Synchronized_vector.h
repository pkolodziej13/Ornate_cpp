#pragma once
#include <cassert>
#include <Aggregations/vector_set.h>

namespace col
{
	template<class T>
	struct Synchronized_vector
	{
		template<class T_specific>
		void emplace_for(const agg::vector_set<T_specific>& sorted_synchronized, const T_specific& added, const T& to_emplace)
		{
			assert((elements.size() == (sorted_synchronized.size() - 1)));
			size_t place_of_new = sorted_synchronized.distance_of(added);
			elements.insert(elements.begin() + place_of_new, to_emplace);
		}
		template<class T_specific>
		void set_for(const agg::vector_set<T_specific>& sorted_synchronized, const T_specific& selected, const T& to_set)
		{
			size_t place = sorted_synchronized.distance_of(selected);
			elements[place] = to_set;
		}
		template<class T_specific>
		const T& get(const agg::vector_set<T_specific>& sorted_synchronized, const T_specific& selected)
		{
			size_t place = sorted_synchronized.distance_of(selected);
			return elements[place];
		}
		template<class T_specific>
		void remove_element_of(const agg::vector_set<T_specific>& sorted_synchronized, const T_specific& selected)
		{
			size_t place = sorted_synchronized.distance_of(selected);
			elements.erase(elements.begin() + place);
		}
	private:
		std::vector<T> elements;
	};

}