#pragma once
#include "Pack.h"
namespace typ
{
	namespace detail
	{
		template<class P1, class P2>
		struct P_merge;
		template<class ... T1_v, class ... T2_v>
		struct P_merge<Pack<T1_v...>, Pack<T2_v...>>
		{
			using type = Pack<T1_v..., T2_v...>;
		};

		template<class P> struct P_exclusion;
		template<class T, class ...T_v> 
		struct P_exclusion<Pack<T, T_v...>>
		{
			using exluted = typename  P_exclusion<Pack<T_v...>>::type;
			using type = std::conditional_t<p_has<exluted, T>,
				exluted, 
				typename P_merge<exluted, Pack<T>>::type>;
		};
		template<> 
		struct P_exclusion<Pack<>>
		{
			using type = Pack<>;
		};
		template<size_t i, class P>
		struct P_delete_at_position;
		template< class T, class ...T_v>
		struct P_delete_at_position<0, Pack<T, T_v...>>
		{
			using type = Pack<T_v...>;
		};
		template<size_t i, class T, class ...T_v>
		struct P_delete_at_position<i, Pack<T, T_v...>>
		{
			using type = typename P_merge < Pack<T>, typename P_delete_at_position<i - 1, Pack<T_v...>>::type>::type;
		};
		template<size_t i>
		struct P_delete_at_position<i, Pack<>>
		{
			using type = Pack<>;
		};

		template< class T, class P, class Enable = void>
		struct P_delete_type;
		template< class T, class P>
		struct P_delete_type< T, P, std::enable_if_t<p_has<P, T  >>>
		{
			using del_type = typename  P_delete_at_position< p_index<P, T>, P >::type;
			using type = typename P_delete_type<T, P>::type;
		};
		template< class T, class P>
		struct P_delete_type< T, P, std::enable_if_t<!p_has<P, T  >>>
		{
			using type = P;
		};

		template <class P1, class P2>
		struct P_conjunction;
		template <class P1, class T, class ... T_v>
		struct P_conjunction<P1, Pack<T, T_v...>>
		{
			using type = std::conditional_t<p_has<P1, T>,
				typename P_merge<Pack<T>, typename P_conjunction<P1, Pack<T_v...>>::type>::type,
				typename P_conjunction<P1, Pack<T_v...>>::type>;
		};
		template <class P1>
		struct P_conjunction<P1, Pack<>>
		{
			using type = Pack<>;
		};

		template<class Types_to_detach, class Pack_to_change>
		struct P_detach;
		template<class Types_to_detach, class T, class ...T_v>
		struct P_detach< Types_to_detach, Pack<T, T_v...>  >
		{
			using pom = typename P_detach<Types_to_detach, Pack<T_v...>>::type;
			using type = std::conditional_t<p_has<Types_to_detach, T>,
				pom, 
				typename P_merge<Pack<T>, pom>::type>;
		};
		template<class Types_to_detach>
		struct P_detach< Types_to_detach, Pack<>  >
		{
			using type = Pack<>;
		};

		template<class P, size_t start>
		struct P_cut_first;
		template<class T, class ... T_v, size_t start>
		struct P_cut_first<Pack<T, T_v...>, start>
		{
			using type = typename P_cut_first<Pack<T_v...>, start - 1>::type;
		};
		template<class T, class ... T_v>
		struct P_cut_first<Pack<T, T_v...>, 0>
		{
			using type = Pack<T, T_v...>;
		};
		template<>
		struct P_cut_first<Pack<>, 0>
		{
			using type = Pack<>;
		};

		template<class P, size_t end>
		struct P_cut_last;
		template<class T, class ... T_v, size_t end>
		struct P_cut_last<Pack< T, T_v...>, end>
		{
			using type = typename P_merge<Pack<T>, typename P_cut_last<Pack<T_v...>, end - 1>::type>::type;
		};
		template<class T, class ... T_v>
		struct P_cut_last<Pack<T, T_v...>, 0>
		{
			using type = Pack<>;
		};
		template< >
		struct P_cut_last<Pack<  >, 0>
		{
			using type = Pack<>;
		};
	}
	template<class P1, class P2>
	using p_merge = typename detail::P_merge<P1, P2>::type;

	template<class P>
	using p_exclusion = typename detail::P_exclusion< P>::type;

	template<size_t i, class P>
	using p_delete_at = typename detail::P_delete_at_position<i,P>::type;

	template <class T, class P>
	using p_delete_type = typename detail::P_delete_type<T, P>::type;

	template<class P1, class P2>
	using p_conjunction = p_exclusion<  typename  detail::P_conjunction<P1, P2>::type>;

	template<class Pack_to_change, class Types_to_detach>
	using p_detach = typename detail::P_detach< Types_to_detach, Pack_to_change >::type;

	template<class P, size_t start, size_t end>
	using p_sub = typename detail::P_cut_first< typename detail::P_cut_last<P, end>::type, start>::type;

	template<class P1, class P2>
	using p_disjunction = p_exclusion<p_merge<P1, P2>>;
}



