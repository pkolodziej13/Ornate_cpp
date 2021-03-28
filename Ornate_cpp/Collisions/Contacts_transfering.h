#pragma once
#include <bitset>
#include "Collisions_requirements.h"
namespace col
{
	template<class Comparer>
	struct Contacts_transfering
	{
		using Layers_p = typename Comparer::Layers_p;
		using Layers_key = std::bitset<Layers_p::size>;
		struct Elemnts_parsed_info
		{
			size_t first_rejestr_element;
			size_t second_rejestr_element;
			size_t collision_number;
			bool not_reversed;
		};
		template<class T>
		static constexpr Layers_key colliding_map_of_type()
		{
			Layers_key result;
			auto collect_collides = [&](auto par)
			{
				using type = decl_parameter(par);
				if (Comparer::template collides<T, type>() || Comparer::template collides< type, T>())
					result.set(typ::p_index < Layers_p, type>);
			};
			Layers_p::for_each(collect_collides);
			return result;
		}
		static std::vector<size_t> collisions_of_keys(const Layers_key& key)
		{
			std::vector<size_t> result;
			for_i(Layers_p::size)
				if (key.test(i))
					result.push_back(i);
			return result;
		}
		static size_t collision_number(size_t i, size_t j)
		{
			size_t min = std::min<size_t>(i, j);
			size_t max = std::max<size_t>(i, j);
			for_i(Comparer::collisons_number)
			{
				if (Comparer::collisions_list[i][0] == min && Comparer::collisions_list[i][1] == max)
					return i;
			}
			return 0;
		}
		static std::vector<Elemnts_parsed_info> get_connections_of_keys(Layers_key key1, Layers_key key2)
		{
			std::vector<Elemnts_parsed_info> result;

			auto layers1 = collisions_of_keys(key1);
			auto layers2 = collisions_of_keys(key2);

			for_i(layers1.size())
			{
				size_t last_used_in_second = 0;
				for_j_scope(last_used_in_second, layers2.size())
				{
					if (layers1[i] <= layers2[j])
					{
						if (Comparer::layer_of_indexes_collides(layers1[i], layers2[j]))
							result.push_back({ i,j,collision_number(layers1[i], layers2[j]),layers1[i] <= layers2[j] });
					}
					else
						last_used_in_second = j + 1;
				}
			}
			return result;
		}
	};

}