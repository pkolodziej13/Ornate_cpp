#pragma once
#include <tuple>

#include "Pack_transformations.h"
#include "Members.h"

namespace typ
{
	namespace detail
	{
		template<class p>
		struct Pack_synthesis
		{
			using type = p;
		};
		template<class t>
		struct Pack_synthesis<Pack<t>>
		{
			using type = t;
		};
		template<>
		struct Pack_synthesis<Pack<>>
		{
			using type = void;
		};

		template<class p>
		struct P_ensure
		{
			using type = Pack<p>;
		};
		template<class ... pp>
		struct P_ensure<Pack<pp...>>
		{
			using type = Pack<pp ...>;
		};

		template<class p1, class p2>
		struct P_matches;
		template<class ...v1, class ... v2>
		struct P_matches<Pack<v1...>, Pack<v2...>>
		{
			static constexpr bool result()
			{
				return ((p_has<Pack<v1...>, v2>) && ...) && ((p_has<Pack<v2...>, v1>) && ...);
			}
			static constexpr bool value = result();
		};

		template< class t1, class t2>
		struct P_add
		{
			using type = Pack <t1, t2 >;
		};
		template< class ... tt1, class ... tt2>
		struct P_add< Pack<tt1...>, Pack<tt2...>>
		{
			using type = Pack < tt1..., tt2... >;
		};
		template< class ... tt, class t2>
		struct P_add< Pack<tt...>, t2>
		{
			using type = Pack < tt..., t2 >;
		};
		template< class t2, class ... tt>
		struct P_add< t2, Pack<tt...>>
		{
			using type = Pack <t2, tt... >;
		};

		template<class ...tt>
		struct P_fusion;
		template<class t1, class ... tt>
		struct P_fusion< t1, tt...>
		{
			using type = typename P_add< t1, typename  P_fusion<tt...>::type>::type;
		};
		template<>
		struct P_fusion<>
		{
			using type = Pack<>;
		};

		template<class p>
		struct P_collapse
		{
			using type = Pack<p>;
		};
		template<class ... pp>
		struct P_collapse<Pack<pp...>>
		{
			using type = typename P_fusion<typename  P_collapse<pp>::type ...>::type;
		};
	}


	template<class p>
	using p_synthesis = typename detail::Pack_synthesis<p>::type;

	template<class p>
	using p_ensure = typename detail::P_ensure<p>::type;

	template<class p1, class p2>
	constexpr bool p_matches = detail::P_matches<p1, p2>::value;

	template<class p, class t>
	using p_add = typename detail::P_add<p, t>::type;

	template<class ... pp>
	using p_fusion = typename detail::P_fusion<pp...>::type;

	template<class ... pp>
	using p_collapse = typename detail::P_collapse<Pack<pp...> >::type;

	template<class p>
	using p_tuple = p_transform<std::tuple, p>;
}

#define packalize_member(member)    \
	has_member_type(member);																							\
template<typename T, class Enable = void  >																				\
struct Packalize_##member{ using type = ::typ::Pack<>; };																		\
template<typename T>																									\
struct Packalize_##member<T, ::std::enable_if_t<has_##member<T>::value>>														\
{																														\
	using type = std::conditional_t<::typ::is_pack<typename T::##member>, typename T::##member, ::typ::Pack<typename T::##member>   >; \
};																														\
template<class t> using packalize_##member = typename Packalize_##member<t>::type;