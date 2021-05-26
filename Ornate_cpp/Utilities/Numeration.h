#pragma once
#include <Types/Pack_indexing.h>
#include <Types/Pack_transformations.h>

#include "Utility.h"
#include "Numbered_components.h"


namespace uti
{
	namespace detail
	{
		template<class T_p, class Sequence>
		struct Transform_to_numbered_elemnts;
		template<class ... T_v, size_t ... i_v>
		struct Transform_to_numbered_elemnts<typ::Pack<T_v...>, ::std::index_sequence<i_v...>>
		{
			using type = typ::Pack<uti::Numbered_component<T_v, i_v>...>;
		};
		template<class T>
		using Non_void = std::integral_constant<bool, !std::is_same_v<typename T::type, void>>;

		template<class T_p>
		using Types_sequence_to_numbered_elements = typ::p_select<Non_void, typ::type_of<Transform_to_numbered_elemnts<T_p, std::make_index_sequence<T_p::size>>>>;

		template<class T_p>
		struct Numbered_elements;
		template<class ... T_v>
		struct Numbered_elements<typ::Pack<T_v...>>
			:uti::default_constructor,
			T_v...
		{
			using uti::default_constructor::default_constructor;
			Numbered_elements(const typ::type_of<T_v> & ... args) :T_v(args)...
			{}

			template<class Lambda>
			void for_each(Lambda&& lambda)
			{
				(lambda(((T_v&)(*this)).get()),...);
			}
			template<class Lambda>
			auto expand(Lambda&& lambda)
			{
				return lambda(T_v::get()...);
			}
			template<class Lambda>
			auto expand(Lambda&& lambda)const
			{
				return lambda(T_v::get()...);
			}

		};
	}


	template<class ... Types_v>
	struct Numeration
		:detail::Numbered_elements<detail::Types_sequence_to_numbered_elements<typ::Pack<Types_v...>>>
	{
		using base_type = detail::Numbered_elements<detail::Types_sequence_to_numbered_elements<typ::Pack<Types_v...>>>;
		using base_type::Numbered_elements;
		using Types_p = typ::Pack<Types_v...>;

		template<size_t i >
		using Component = uti::Numbered_component< typ::p_element<i, Types_p>, i>;
		template<class T >
		using Component_index_t = typ::detail::P_index<Types_p, T>;

		static constexpr size_t size = Types_p::size;

		template<size_t i>
		auto& get()
		{
			using component_type = Component< i>;
			return this->component_type::get();
		}
		template<size_t i>
		const auto& get() const
		{
			using component_type = Component< i>;
			return this->component_type::get();
		}
		template<class T>
		std::enable_if_t<typ::p_has<Types_p, T>, T&>  get()
		{
			using component_type = Component<typ::p_index< Types_p, T>>;
			return this->component_type::get();
		}
		template<class T>
		std::enable_if_t<typ::p_has<Types_p, T>, const T&>  get()const
		{
			using component_type = Component<typ::p_index< Types_p, T>>;
			return this->component_type::get();
		}

	};
}

