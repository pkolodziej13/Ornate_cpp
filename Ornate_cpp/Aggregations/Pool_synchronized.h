#pragma once
#include <vector>
#include <cassert>
#include <array>

#include <Utilities/Utility.h>
#include <Utilities/loops.h>
#include <Utilities/Range_for_loop.h>

namespace agg
{
	enum class Live_status
	{
		free, living, dead, initiated
	};
	inline Live_status update_live_status(Live_status status)
	{
		switch (status)
		{
		case Live_status::free: return Live_status::free;
		case Live_status::initiated: return Live_status::living;
		case Live_status::living: return Live_status::living;
		case Live_status::dead: return Live_status::free;
		}
		return Live_status::free;
	}

	template <class T>
	struct Pool_synchronized
	{
		using element_type = T;

		size_t total_size()const
		{
			return activities.size();
		}
		size_t used_size()const
		{
			return activities.size() - holes.size();
		}

		Live_status activity(size_t i)const
		{
			return activities[i];
		}

		T & operator [](size_t i)
		{
			assert(activities[i]== Live_status::living);
			return elements[i];
		}
		const T & operator [](size_t i)const
		{
			assert(activities[i] == Live_status::living);
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

		size_t add(const T& t)
		{
			size_t result = activate_place(t);
			elements[result] = t;
			return result;
		}
		void free(size_t i)
		{
			deactivate_place(i);
		}
		void clear()
		{
			elements.clear();
			holes.clear();
			activities.clear();
		}
		void refresh()
		{
			for_i(activities.size())
			{
				if (activities[i] == Live_status::dead)
					holes.push_back(i);
				activities[i] = update_live_status(activities[i]);
			}
		}
		std::vector<std::pair<size_t, size_t>> shrink_to_fit()
		{
			std::vector<std::pair<size_t, size_t>> result;
			result.reserve(holes.size());
			size_t from_begin = 0, form_end = activities.size();

			auto move_active_to_hole = [&](size_t active, size_t hole)
			{
				result.push_back({ active,hole });
				activities[hole] = activities[active];
				activities[active] = Live_status::free;
				elements[hole] = elements[active];
			};
			auto propagete_indexes = [&]()
			{
				while ((activities.size() > from_begin)&(from_begin < form_end)&(activities[from_begin] != Live_status::free))
					++from_begin;
				while (from_begin < form_end&&activities[from_begin] == Live_status::free)
					--form_end;
			};

			propagete_indexes();
			while (from_begin < form_end)
			{
				move_active_to_hole(form_end, from_begin);
				propagete_indexes();
			}
			elements.resize(elements.size()- holes.size());
			activities.resize(elements.size());
			elements.shrink_to_fit();
			activities.shrink_to_fit();
			holes.clear();
			return result;
		}

		struct Living_loop_range
		{
			static constexpr auto features = uti::Loop_features::aggregator_unintacted |
				uti::Loop_features::order_unintacted |
				uti::Loop_features::remove_save;

			Living_loop_range(Pool_synchronized<T>& pool)
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
				while (i < pool.activities.size() && pool.activities[i] != Live_status::living)
					++i;
			}
			size_t i{};
			Pool_synchronized<T>& pool;
		};
		auto active_range()
		{
			return uti::Range_for_loop<Living_loop_range>(*this);
		}
	private:
		size_t activate_place(const T& t)
		{
			if (!holes.empty())
			{
				size_t index = holes.back();
				activities[index] = Live_status::initiated;
				holes.pop_back();
				return index;
			}
			else
			{
				activities.push_back(Live_status::initiated);
				elements.emplace_back(t);
				return elements.size() - 1;
			}
		}
		void deactivate_place(size_t i)
		{
			assert((activities[i] == Live_status::initiated || activities[i] == Live_status::living));
			activities[i] = Live_status::dead;
		}

		std::vector<T> elements;

		std::vector<Live_status> activities;
		std::vector<size_t> holes;
	};

}