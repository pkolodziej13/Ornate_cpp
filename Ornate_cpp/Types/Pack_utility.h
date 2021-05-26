#pragma once
#include <tuple>

#include "Pack_transformations.h"
#include "Members.h"

namespace typ
{
	namespace detail
	{
		template<class P>
		struct Pack_synthesis
		{
			using type = P;
		};
		template<class T>
		struct Pack_synthesis<Pack<T>>
		{
			using type = T;
		};
		template<>
		struct Pack_synthesis<Pack<>>
		{
			using type = void;
		};

		template<class P>
		struct P_ensure
		{
			using type = Pack<P>;
		};
		template<class ... T_v>
		struct P_ensure<Pack<T_v...>>
		{
			using type = Pack<T_v ...>;
		};

		template<class P1, class P2>
		struct P_matches;
		template<class ...T1_v, class ... T2_v>
		struct P_matches<Pack<T1_v...>, Pack<T2_v...>>
		{
			static constexpr bool result()
			{
				return ((p_has<Pack<T1_v...>, T2_v>) && ...) && ((p_has<Pack<T2_v...>, T1_v>) && ...);
			}
			static constexpr bool value = result();
		};

		template< class T1, class T2>
		struct P_add
		{
			using type = Pack <T1, T2 >;
		};
		template< class ... T1_v, class ... T2_v>
		struct P_add< Pack<T1_v...>, Pack<T2_v...>>
		{
			using type = Pack < T1_v..., T2_v... >;
		};
		template< class ... T_v, class T>
		struct P_add< Pack<T_v...>, T>
		{
			using type = Pack < T_v..., T >;
		};
		template< class T, class ... T_v>
		struct P_add< T, Pack<T_v...>>
		{
			using type = Pack <T, T_v... >;
		};

		template<class ...T_v>
		struct P_fusion;
		template<class T, class ... T_v>
		struct P_fusion< T, T_v...>
		{
			using type = typename P_add< T, typename  P_fusion<T_v...>::type>::type;
		};
		template<>
		struct P_fusion<>
		{
			using type = Pack<>;
		};

		template<class T>
		struct P_collapse
		{
			using type = Pack<T>;
		};
		template<class ... T_v>
		struct P_collapse<Pack<T_v...>>
		{
			using type = typename P_fusion<typename  P_collapse<T_v>::type ...>::type;
		};
	}

	template<class P>
	using p_synthesis = typename detail::Pack_synthesis<P>::type;

	template<class P>
	using p_ensure = typename detail::P_ensure<P>::type;

	template<class P1, class P2>
	constexpr bool p_matches = detail::P_matches<P1, P2>::value;

	template<class P1, class P2>
	using p_add = typename detail::P_add<P1, P2>::type;

	template<class ... T_v>
	using p_fusion = typename detail::P_fusion<T_v...>::type;

	template<class ... T_v>
	using p_collapse = typename detail::P_collapse<Pack<T_v...> >::type;

	template<class P>
	using p_tuple = p_transform<std::tuple, P>;
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
template<class T> using packalize_##member = typename Packalize_##member<T>::type;