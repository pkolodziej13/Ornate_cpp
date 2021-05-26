#pragma once
#include "Utility.h"

namespace typ
{
	template< class ...T_v> 
	struct Pack 
	{
		static constexpr size_t size = sizeof...(T_v);

		template<class Lambda>
		constexpr static void for_each(Lambda&& lambda)
		{
			(lambda(Overtype<T_v>()) ,...);
		}
		template<class Lambda>
		constexpr static auto expand(Lambda&& lambda)
		{
			return lambda(Overtype<T_v>()...);
		}
	};

	namespace detail
	{
		template <typename T, typename P>
		struct P_has;
		template <typename T>
		struct P_has<T, Pack<>> : std::false_type 
		{};
		template <typename T1, typename T2, typename... T_v>
		struct P_has<T1, Pack<T2, T_v...>> : P_has<T1, Pack<T_v...>>
		{};
		template <typename T, typename... T_v>
		struct P_has<T, Pack<T, T_v...>> : std::true_type
		{};

		template <class P, class T>
		struct P_index;
		template <class T, class... T_v>
		struct P_index<Pack<T, T_v...>, T> : std::integral_constant<std::size_t, 0>
		{};
		template <class T1, class T2, class... T_v>
		struct P_index<Pack<T2, T_v...>, T1> : std::integral_constant<std::size_t, 1 + P_index<Pack< T_v...>, T1>::value>
		{};
		template <class T>
		struct P_index<Pack<>, T>
		{};

		template<size_t n, class P>
		struct P_element;
		template<size_t n, class T, class ... T_v>
		struct P_element<n, Pack<T, T_v...>>
		{
			using type = std::conditional_t<n == 0, T, typename P_element<n - 1, Pack< T_v...>>::type>;
		};
		template<size_t n, class T >
		struct P_element<n, Pack<T>>
		{
			using type = T;
		};

		template<class T>
		struct P_is : std::false_type {};
		template<class ... T_v>
		struct P_is<Pack<T_v...>> : std::true_type {};
	}

	template<class P, class T>
	constexpr bool p_has = detail::P_has<T, P>::value;

	template<class P, class T>
	constexpr size_t p_index = detail::P_index<P, T>::value;

	template <size_t n, class P>
	using p_element = typename  detail::P_element<n, P>::type;

	template <class P>
	constexpr bool is_pack = detail::P_is<P>::value;
}

