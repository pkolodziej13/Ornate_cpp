#pragma once
#include <map>

namespace agg
{
	template<class T1, class T2 >
	struct Bimap
	{
		void add(const T1& key1, const T2& key2)
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
		void remove(const T1& key)
		{
			T2& key2 = map1.at(key);
			map1.erase(key);
			map2.erase(key2);
		}
		inline void remove(const T2& key)
		{
			T1& key1 = map2.at(key);
			map2.erase(key);
			map1.erase(key1);
		}
		T1& get(const T2& key)
		{
			return map2.at(key);
		}
		T2& get(const T1& key)
		{
			return map1.at(key);
		}
		const T1& operator[](const T2& key)const
		{
			return map2.at(key);
		}
		const T2& operator[](const T1& key)const
		{
			return map1.at(key);
		}
		T1& operator[](const T2& key)
		{
			return map2.at(key);
		}
		T2& operator[](const T1& key)
		{
			return map1.at(key);
		}

		bool has(const T1& key)const
		{
			return map1.count(key) != 0;
		}
		bool has(const T2& key)const
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

		std::map<T1, T2> map1;
		std::map<T2, T1> map2;
	};
}