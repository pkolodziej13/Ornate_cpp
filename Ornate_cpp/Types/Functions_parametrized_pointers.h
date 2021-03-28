#pragma once
#include <utility>
#include <array>

namespace typ
{
	template<class T, size_t I>
	struct Functions_parametrized_pointers
	{
		using Pointer_type = std::add_pointer_t<decltype(T::template call<0>)>;

		template<size_t ... I_sequnce>
		static constexpr std::array<Pointer_type, I> create_array_from_sequence(std::index_sequence<I_sequnce...>)
		{
			return { T::template call<I_sequnce>... };
		}
		static constexpr std::array<Pointer_type, I> create_array()
		{
			return create_array_from_sequence(std::make_index_sequence<I>{});
		}

		static constexpr std::array<Pointer_type, I> value = create_array();
	};
}