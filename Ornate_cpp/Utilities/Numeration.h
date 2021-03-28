#pragma once
#include <Types/Pack_indexing.h>
#include <Types/Pack_transformations.h>

#include "Utility.h"
#include "Numbered_components.h"


namespace uti
{
	namespace detail
	{
		template<class pack, class sequence>
		struct Transform_to_numbered_elemnts;
		template<class ... p_v, size_t ... I_v>
		struct Transform_to_numbered_elemnts<typ::Pack<p_v...>, ::std::index_sequence<I_v...>>
		{
			using type = typ::Pack<uti::Numbered_component<p_v, I_v>...>;
		};
		template<class T>
		using Non_void = std::integral_constant<bool, !std::is_same_v<typename T::type, void>>;

		template<class pack>
		using Types_sequence_to_numbered_elements = typ::p_select<Non_void, typ::type_of<Transform_to_numbered_elemnts<pack, std::make_index_sequence<pack::size>>>>;

		template<class p>
		struct pro_numeration;
		template<class ... pp>
		struct pro_numeration<typ::Pack<pp...>>
			:uti::default_constructor,
			pp...
		{
			template<class l>
			void for_each(l&& lam)
			{
				auto d = { (lam(((pp&)(*this)).get())    ,0)...,0 };
			}
			template<class l>
			auto expand(l&& lam)
			{
				return lam(pp::get()...);
			}
			template<class l>
			auto expand(l&& lam)const
			{
				return lam(pp::get()...);
			}

			pro_numeration(const typ::type_of<pp> & ... args) :pp(args)...
			{}
			using uti::default_constructor::default_constructor;
		};
	}


	template<class ... types_v>
	struct Numeration
		:detail::pro_numeration<detail::Types_sequence_to_numbered_elements<typ::Pack<types_v...>>>
	{
		using base_type = detail::pro_numeration<detail::Types_sequence_to_numbered_elements<typ::Pack<types_v...>>>;
		using base_type::pro_numeration;
		using types_p = typ::Pack<types_v...>;

		template<size_t i >
		using component = uti::Numbered_component< typ::p_element<i, types_p>, i>;
		template<class t >
		using index = typ::detail::P_index<types_p, t>;

		static constexpr size_t size = types_p::size;

		template<size_t i>
		auto& get()
		{
			using component_type = component< i>;
			return this->component_type::get();
		}
		template<size_t i>
		const auto& get() const
		{
			using component_type = component< i>;
			return this->component_type::get();
		}
		template<class t>
		std::enable_if_t<typ::p_has<types_p, t>, t&>  get()
		{
			using component_type = component<typ::p_index< types_p, t>>;
			return this->component_type::get();
		}
		template<class t>
		std::enable_if_t<typ::p_has<types_p, t>, const t&>  get()const
		{
			using component_type = component<typ::p_index< types_p, t>>;
			return this->component_type::get();
		}

	};
}

