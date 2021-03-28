#pragma once
#include <vector>
#include <algorithm>
namespace agg
{
	template<class T>
	struct vector_set
	{
		bool add(const T& element)
		{
			auto place_position = std::lower_bound(elements.begin(), elements.end(), element);
			if (place_position != elements.end() && (*place_position) == element)
				return false;
			elements.insert(place_position, element);
			return true;
		}
		size_t distance_of(const T& element)const
		{
			return std::distance(elements.begin(), std::lower_bound(elements.begin(), elements.end(), element));
		}
		void remove(const T& element)
		{
			auto place_position = std::lower_bound(elements.begin(), elements.end(), element);
			if (place_position != elements.end() && (*place_position) == element)
				elements.erase(place_position);
		}
		void remove_at_position(size_t position)
		{
			if (elements.size()> position)
				elements.erase(elements.begin()+ position);
		}
		size_t size()const
		{
			return elements.size();
		}
		const std::vector<T>& get_elements()const
		{
			return elements;
		}
		void clear()
		{
			elements.clear();
		}
		T & operator[](size_t i)
		{
			return elements[i];
		}
		const T& operator[](size_t i)const
		{
			return elements[i];
		}
		vector_set<T> merge(const vector_set<T>& to_merge)const
		{
			if (this->size() == 0)
				return to_merge;
			else if (to_merge.size() == 0)
				return *this;

			size_t i = 0;
			size_t j = 0;
			vector_set<T> result;
			bool not_completed = true;
			auto copy_rest_form = [&](const vector_set<T>& to_copy, size_t index)
			{
				for (; index < to_copy.size(); ++index)
					result.add_suerly_as_last(to_copy[index]);
				not_completed = false;
			};

			while (not_completed)
			{
				if (elements[i] == to_merge[j])
				{
					result.add_suerly_as_last(elements[i]);
					++i;
					++j;
					if (i == elements.size())
						copy_rest_form(to_merge,j);
					if (j == to_merge.size())
						copy_rest_form(*this, i);
				}
				else if (elements[i] < to_merge[j])
				{
					result.add_suerly_as_last(elements[i]);
					++i;
					if (i == elements.size())
						copy_rest_form(to_merge, j);
				}
				else if (elements[i] > to_merge[j])
				{
					result.add_suerly_as_last(to_merge[j]);
					++j;
					if (j == to_merge.size())
						copy_rest_form(*this, i);
				}
			}
			return result;
		}
		bool contains(const T& element)const
		{
			return std::binary_search(elements.begin(), elements.end(), element);
		}

	private:
		void add_suerly_as_last(const T& element)
		{
			elements.push_back(element);
		}

		std::vector<T> elements;
	};
}