#pragma once
#include <Types/Pack.h>
#include <Aggregations/Symetric_value_table.h>

namespace col
{
	template<class ...Layers_v>
	struct Engine_layers_comparer
	{
		using Layers_p = typ::Pack<Layers_v...>;
		constexpr Engine_layers_comparer<Layers_v...>& comparer()
		{
			return *this;
		}
		using Comparer_type = Engine_layers_comparer<Layers_v...>;
		template<class Layer_specific>
		static size_t layer_key()
		{
			return typ::p_index<Layers_p, Layer_specific>;
		}
		static bool layer_of_indexes_collides(size_t a, size_t b)
		{
			return collisions_table.get(a, b);
		}
		template<class Layer_left, class Layer_right>
		static constexpr bool collides()
		{
			return typ::p_has< typename Layer_left::collidable, Layer_right>;
		}
		static constexpr agg::Symetric_value_table<bool, Layers_p::size> create_collisions_table()
		{
			agg::Symetric_value_table<bool, Layers_p::size> result(false);
			Layers_p::for_each([&](auto L1)
			{
				using Layer1 = decl_parameter(L1);
				Layers_p::for_each([&](auto L2)
				{
					using Layer2 = decl_parameter(L2);
					if (collides<Layer1, Layer2>())
						result.set(typ::p_index<Layers_p, Layer1>, typ::p_index<Layers_p, Layer2>, true);
				});
			});
			return result;
		}
		static constexpr agg::Symetric_value_table<bool, Layers_p::size> collisions_table = create_collisions_table();

		static constexpr size_t calculate_collisions_number()
		{
			size_t result = 0;
			collisions_table.for_each([&](bool c) { result += c; });
			return result;
		}
		static constexpr size_t collisons_number = calculate_collisions_number();
		static constexpr std::array<std::array<size_t, 2>, collisons_number> callculate_colisions_layers_pairs()
		{
			std::array<std::array<size_t, 2>, collisons_number> result{};
			size_t k = 0;
			for (size_t i = 0; i < Layers_p::size; ++i)
				for (size_t j = i; j < Layers_p::size; ++j)
					if (collisions_table.get(i, j) == true)
					{
						result[k][0] = std::min<size_t>(i, j);
						result[k][1] = std::max<size_t>(i, j);
						++k;
					}
			return result;
		}
		static constexpr std::array<std::array<size_t, 2>, collisons_number> collisions_list = callculate_colisions_layers_pairs();
		static constexpr size_t collisions_list_array_access_repair(size_t i, size_t j)
		{
			return collisions_list[i][j];
		}
		template<class T>
		struct Array_to_pack;
		template<size_t ... I>
		struct Array_to_pack<std::index_sequence<I...>>
		{
			using type = typ::Pack<typ::Pack<
				typ::p_element<collisions_list_array_access_repair(I, 0), Layers_p>,
				typ::p_element<collisions_list_array_access_repair(I, 1), Layers_p>>...>;
		};
		using Layers_collisions_p = typename Array_to_pack<std::make_index_sequence<collisons_number>>::type;


		template<class Colliding>
		using Selector_of_colliding = std::integral_constant<bool, typ::p_has<Layers_p, Colliding>>;
		template<class T>
		using Colliding_objects = typ::p_select<Selector_of_colliding, typename T::collidable >;

	};

}