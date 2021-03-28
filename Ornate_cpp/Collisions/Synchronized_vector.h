#pragma once
#include <cassert>
#include <Aggregations/vector_set.h>

namespace col
{
	template<class T>
	struct Synchronized_vector
	{
		template<class TT>
		void emplace_for(const agg::vector_set<TT>& sorted_synchronized, const TT& added, const T& to_emplace)
		{
			assert((elements.size() == (sorted_synchronized.size() - 1)));
			size_t place_of_new = sorted_synchronized.distance_of(added);
			elements.insert(elements.begin() + place_of_new, to_emplace);
		}
		template<class TT>
		void set_for(const agg::vector_set<TT>& sorted_synchronized, const TT& selected, const T& to_set)
		{
			size_t place = sorted_synchronized.distance_of(selected);
			elements[place] = to_set;
		}
		template<class TT>
		const T& get(const agg::vector_set<TT>& sorted_synchronized, const TT& selected)
		{
			size_t place = sorted_synchronized.distance_of(selected);
			return elements[place];
		}
		template<class TT>
		void remove_element_of(const agg::vector_set<TT>& sorted_synchronized, const TT& selected)
		{
			size_t place = sorted_synchronized.distance_of(selected);
			elements.erase(elements.begin() + place);
		}
		std::vector<T> elements;
	};

}