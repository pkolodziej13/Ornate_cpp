#pragma once
#include <vector>
#include <Utilities/loops.h>

namespace agg
{
	template<class T>
	struct Dense
	{
		size_t add(const T & e)
		{
			elements.push_back(e);
			return elements.size() - 1;
		}
		void remove_element(const T &e)
		{
			assert(has(e));
			swap_remove_at(index_of_element(e));
		}
		size_t index_of_element(const T &e)const
		{
			for_i(elements.size())
			{
				if (elements[i] == e)
				{
					return i;
				}
			}
			return -1;
		}
		void swap_remove_at(size_t i)
		{
			elements[i] = elements.back();
			elements.pop_back();
		}
		bool has(const T & e)const
		{
			return index_of_element(e) != -1;
		}
		T & operator[](size_t i)
		{
			return elements[i];
		}

		auto begin()
		{
			return elements.begin();
		}
		auto end()
		{
			return elements.end();
		}
		auto begin()const
		{
			return elements.begin();
		}
		auto end()const
		{
			return elements.end();
		}
		size_t size()const
		{
			return elements.size();
		}
		void clear()
		{
			elements.clear();
		}

	private:
		std::vector<T> elements;
	};
}