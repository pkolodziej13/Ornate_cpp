#pragma once
#include <set>

#include <Utilities/loops.h>

namespace agg
{
	struct Pooled_indexes
	{
		size_t size()const
		{
			return pool_size;
		}
		size_t free_size()const
		{
			return holes.size();
		}
		bool is_free_at(size_t i)const
		{
			if (i < pool_size && holes.count(i) == 0)
			{
				return false;
			}
			return true;
		}
		size_t add()
		{
			if (!holes.empty())
			{
				size_t index = *holes.begin();
				holes.erase(index);
				return index;
			}
			else
			{
				return pool_size++;
			}
		}
		void add_free(size_t siz)
		{
			for_i(siz)
				holes.insert(pool_size++);
		}
		bool try_add_at(size_t i)
		{
			if (i < pool_size)
			{
				if (holes.count(i) == 1)
				{
					holes.erase(i);
					return true;
				}
				return false;
			}
			size_t p_s = size();
			add_free(i- p_s);
			pool_size++;
			return true;
		}
		void free(size_t i)
		{
			holes.insert(i);
		}
		void clear()
		{
			pool_size = 0;
			holes.clear();
		}
		void fit_to_last()
		{
			while (holes.count(pool_size - 1))
			{
				holes.erase(--pool_size);
			}
		}
	protected:
		std::set<size_t> holes;
		size_t pool_size = 0;
	};


}