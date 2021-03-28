#pragma once
#include "Utility.h"

namespace typ
{
	template< class ...tt> 
	struct Pack 
	{
		static constexpr size_t size = sizeof...(tt);
		template<class lam>
		constexpr static void for_each(lam && f)
		{
			( f(Overtype<tt>()) ,...);
		}
		template<class lam>
		constexpr static auto expand(lam && f)
		{
			return f(Overtype<tt>()...);
		}
	};

	namespace detail
	{
		template <typename T, typename p>
		struct P_has;
		template <typename T>
		struct P_has<T, Pack<>> : std::false_type 
		{};
		template <typename T, typename U, typename... Ts>
		struct P_has<T, Pack<U, Ts...>> : P_has<T, Pack<Ts...>> 
		{};
		template <typename T, typename... Ts>
		struct P_has<T, Pack<T, Ts...>> : std::true_type 
		{};

		template <class P, class T>
		struct P_index;
		template <class T, class... Ts>
		struct P_index<Pack<T, Ts...>, T> : std::integral_constant<std::size_t, 0>
		{};
		template <class T, class U, class... Ts>
		struct P_index<Pack<U, Ts...>, T> : std::integral_constant<std::size_t, 1 + P_index<Pack< Ts...>, T>::value>
		{};
		template <class T>
		struct P_index<Pack<>, T>
		{};

		template<size_t n, class p>
		struct P_element;
		template<size_t n, class e1, class ... e>
		struct P_element<n, Pack<e1, e...>>
		{
			using type = std::conditional_t<n == 0, e1, typename P_element<n - 1, Pack< e...>>::type>;
		};
		template<size_t n, class e1 >
		struct P_element<n, Pack<e1>>
		{
			using type = e1;
		};

		template<class t>
		struct P_is : std::false_type {};
		template<class ... pp>
		struct P_is<Pack<pp...>> : std::true_type {};
	}

	template<class p, class t>
	constexpr bool p_has = detail::P_has<t,p>::value;

	template<class p, class t>
	constexpr size_t p_index = detail::P_index<p,t>::value;

	template <size_t n, class p>
	using p_element = typename  detail::P_element<n, p>::type;

	template <class p>
	constexpr bool is_pack = detail::P_is<p>::value;
}

