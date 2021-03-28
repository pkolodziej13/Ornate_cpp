#pragma once
#include <vector>
#include <cassert>

#include <Utilities/Utility.h>
#include <Utilities/loops.h>
#include <Utilities/Range_for_loop.h>

namespace agg
{
	template <class T>
	struct Pool_unordered
	{
		using element_type = T;

		size_t add(const T & t)
		{
			size_t result = activate_place();
			elements[result] = t;
			return result;
		}
		void free(size_t i)
		{
			deactivate_place(i);
		}
		size_t activities_size()const
		{
			return activities.size() - holes.size();
		}
		size_t total_size()const
		{
			return activities.size();
		}
		bool is_active(size_t i)const
		{
			return activities[i];
		}
		T & operator [](size_t i)
		{
			assert(activities[i]);
			return elements[i];
		}
		const T & operator [](size_t i)const
		{
			assert(activities[i]);
			return elements[i];
		}
		T & active_or_not_at(size_t i)
		{
			return elements[i];
		}
		const T & active_or_not_at(size_t i)const
		{
			return elements[i];
		}

		void clear()
		{
			elements.clear();
			holes.clear();
			activities.clear();
		}
		std::vector<std::pair<uint32_t, uint32_t>> shrink_to_fit()
		{
			std::vector<std::pair<uint32_t, uint32_t>> result;
			result.reserve(holes.size());
			size_t from_begin = 0, form_end = activities.size();

			auto move_active_to_hole = [&](size_t active, size_t hole)
			{
				result.push_back({ active,hole });
				activities[hole] = true;
				activities[active] = false;
				elements[hole] = elements[active];
			};
			auto propagete_indexes = [&]()
			{
				while ((activities.size() > from_begin)&(from_begin < form_end)&(activities[from_begin] == true))
					++from_begin;
				while (from_begin < form_end&&activities[from_begin] == false)
					--form_end;
			};

			propagete_indexes();
			while (from_begin < form_end)
			{
				move_active_to_hole(form_end, from_begin);
				propagete_indexes();
			}
			elements.resize(activities_size());
			activities.resize(elements.size());
			elements.shrink_to_fit();
			activities.shrink_to_fit();
			holes.clear();
			return result;
		}

		struct Activities_loop_range
		{
			static constexpr auto features = uti::Loop_features::aggregator_unintacted | 
				uti::Loop_features::order_unintacted|
				uti::Loop_features::remove_save;

			Activities_loop_range(Pool_unordered<T>& pool)
				:pool{ pool }
			{
				find_nearest_active();
			}
			bool finished()const
			{
				return i >= pool.elements.size();
			}
			T& current_element()const
			{
				return pool[i];
			}
			void increment()
			{
				++i;
				find_nearest_active();
			}
			void finish_after_increment()
			{
				i = pool.activities.size();
			}
		private:
			void find_nearest_active()
			{
				while (i < pool.activities.size() && pool.activities[i] == false)
					++i;
			}
			size_t i{};
			Pool_unordered<T> & pool;
		};
		auto active_range()
		{
			return uti::Range_for_loop<Activities_loop_range>(*this);
		}
	private:
		size_t activate_place()
		{
			if (!holes.empty())
			{
				size_t index = holes.back();
				activities[index] = true;
				holes.pop_back();
				return index;
			}
			else
			{
				activities.push_back(true);
				elements.emplace_back();
				return elements.size() - 1;
			}
		}
		void deactivate_place(size_t i)
		{
			assert(activities[i] == true );
			holes.push_back(i);
			activities[i] = false;
		}

		std::vector<T> elements;

		std::vector<bool> activities;
		std::vector<size_t> holes;
	};
}