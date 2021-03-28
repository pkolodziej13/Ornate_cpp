#pragma once
#include <type_traits>

#define for_named_range(name,size) for(std::decay_t<decltype(size)> name = 0; name < (size) ; name++)
#define for_named_section(name,start,end) for(std::decay_t<decltype(start)> name = (start); name <(end); name ++)
#define for_i(size) for_named_range(i,size)
#define for_j(size) for_named_range(j,size)
#define for_k(size) for_named_range(k,size)
#define for_i_scope(start,end) for_named_section(i,start,end)
#define for_j_scope(start,end) for_named_section(j,start,end)
#define for_k_scope(start,end) for_named_section(k,start,end)

namespace uti
{
	namespace detail
	{
		template<class Lambda, size_t ...I>
		void inline call_for_index_sequence(Lambda&& lambda, std::index_sequence<I...>)
		{
			((lambda(std::integral_constant<size_t, I>())), ...);
		}

		template< class ... T_v, class Lambda, size_t ...I>
		inline void call_for_tuple(std::tuple<T_v...>& t, const Lambda& lambda, std::index_sequence<I...>)
		{
			(lambda(std::get<I>(t)), ...);
		}
	}

	template<size_t I, class Lambda >
	void inline for_index_sequence(Lambda && lambda)
	{
		detail::call_for_index_sequence(std::forward<Lambda>(lambda), std::make_index_sequence<I>());
	}
	template<class ... T_v, class Lambda>
	inline void for_tuple(std::tuple<T_v...>& t, Lambda&& lambda)
	{
		detail::call_for_tuple(t, std::forward<Lambda>(lambda), std::make_index_sequence<sizeof...(T_v)>{});
	}


	enum Loop_features
	{
		excludes_added         = 1,
		remove_save            = 1<<1,
		aggregator_remove_save = 1<<2,
		aggregator_unintacted  = 1<<3,
		order_unintacted       = 1<<4,
	};
}