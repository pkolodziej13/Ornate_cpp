#pragma once
#include <map>

namespace agg
{
	template<class t1, class t2 >
	struct bimap
	{
		void add(const t1& key1, const t2& key2)
		{
			if (map1.count(key1))
			{
				remove(key1);
			}
			if (map2.count(key2))
			{
				remove(key2);
			}

			map1[key1] = key2;
			map2[key2] = key1;
		}
		void remove(const t1& key)
		{
			t2& key2 = map1.at(key);
			map1.erase(key);
			map2.erase(key2);
		}
		inline void remove(const t2& key)
		{
			t1& key1 = map2.at(key);
			map2.erase(key);
			map1.erase(key1);
		}
		t1& get(const t2& key)
		{
			return map2.at(key);
		}
		t2& get(const t1& key)
		{
			return map1.at(key);
		}
		const t1& operator[](const t2& key)const
		{
			return map2.at(key);
		}
		const t2& operator[](const t1& key)const
		{
			return map1.at(key);
		}
		t1& operator[](const t2& key)
		{
			return map2.at(key);
		}
		t2& operator[](const t1& key)
		{
			return map1.at(key);
		}

		bool has(const t1& key)const
		{
			return map1.count(key) != 0;
		}
		bool has(const t2& key)const
		{
			return map2.count(key) != 0;
		}

		auto begin()const
		{
			return map1.begin();
		}
		auto end()const
		{
			return map1.end();
		}

		size_t size()const
		{
			return map1.size();
		}
		void clear()
		{
			map1.clear();
			map2.clear();
		}
	private:

		std::map<t1, t2> map1;
		std::map<t2, t1> map2;
	};
}