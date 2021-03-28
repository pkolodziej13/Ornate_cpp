#pragma once
#include "Pack.h"
namespace typ
{
	namespace detail
	{
		template<class P1, class P2>
		struct P_merge;
		template<class ... p1_v, class ... p2_v>
		struct P_merge<Pack<p1_v...>, Pack<p2_v...>>
		{
			using type = Pack<p1_v..., p2_v...>;
		};

		template<class p> struct P_exclusion;
		template<class p1, class ...pp> 
		struct P_exclusion<Pack<p1, pp...>>
		{
			using exluted = typename  P_exclusion<Pack<pp...>>::type;
			using type = std::conditional_t<p_has<exluted,p1>,
				exluted, 
				typename P_merge<exluted, Pack<p1>>::type>;
		};
		template<> 
		struct P_exclusion<Pack<>>
		{
			using type = Pack<>;
		};
		template<size_t i, class p>
		struct P_delete_at_position;
		template< class p, class ...pp>
		struct P_delete_at_position<0, Pack<p, pp...>>
		{
			using type = Pack<pp...>;
		};
		template<size_t i, class p, class ...pp>
		struct P_delete_at_position<i, Pack<p, pp...>>
		{
			using type = typename P_merge < Pack<p>, typename P_delete_at_position<i - 1, Pack<pp...>>::type>::type;
		};
		template<size_t i>
		struct P_delete_at_position<i, Pack<>>
		{
			using type = Pack<>;
		};

		template< class t, class p, class Enable = void>
		struct P_delete_type;
		template< class t, class p>
		struct P_delete_type< t, p, std::enable_if_t<p_has<p, t  >>>
		{
			using del_type = typename  P_delete_at_position< p_index<p,t>, p >::type;
			using type = typename P_delete_type<t, p>::type;
		};
		template< class t, class p>
		struct P_delete_type< t, p, std::enable_if_t<!p_has<p, t  >>>
		{
			using type = p;
		};

		template <class p1, class p2>
		struct P_conjunction;
		template <class p1, class pc, class ... pp>
		struct P_conjunction<p1, Pack<pc, pp...>>
		{
			using type = std::conditional_t<p_has<p1,pc>,
				typename P_merge<Pack<pc>, typename P_conjunction<p1, Pack<pp...>>::type>::type, 
				typename P_conjunction<p1, Pack<pp...>>::type>;
		};
		template <class p1>
		struct P_conjunction<p1, Pack<>>
		{
			using type = Pack<>;
		};

		template< class det, class main_pack>
		struct P_detach;
		template<class det, class m1, class ...mm>
		struct P_detach< det, Pack<m1, mm...>  >
		{
			using pom = typename P_detach<det, Pack<mm...>>::type;
			using type = std::conditional_t<p_has<det, m1>, 
				pom, 
				typename P_merge<Pack<m1>, pom>::type>;
		};
		template<class det>
		struct P_detach< det, Pack<>  >
		{
			using type = Pack<>;
		};

		template<class numeric, size_t start>
		struct P_cut_first;
		template<class n1, class ... nn, size_t start>
		struct P_cut_first<Pack<n1, nn...>, start>
		{
			using type = typename P_cut_first<Pack<nn...>, start - 1>::type;
		};
		template<class n1, class ... nn>
		struct P_cut_first<Pack<n1, nn...>, 0>
		{
			using type = Pack<n1, nn...>;
		};
		template<>
		struct P_cut_first<Pack<>, 0>
		{
			using type = Pack<>;
		};

		template<class numeric, size_t end>
		struct P_cut_last;
		template<class n1, class ... nn, size_t end>
		struct P_cut_last<Pack< n1, nn...>, end>
		{
			using type = typename P_merge<Pack<n1>, typename P_cut_last<Pack<nn...>, end - 1>::type>::type;
		};
		template<class n1, class ... nn>
		struct P_cut_last<Pack<n1, nn...>, 0>
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

	template<class p>
	using p_exclusion = typename detail::P_exclusion<p>::type;

	template<size_t i, class p>
	using p_delete_at = typename detail::P_delete_at_position<i, p>::type;

	template <class t, class p>
	using p_delete_type = typename detail::P_delete_type<t, p>::type;

	template<class p1, class p2>
	using p_conjunction = p_exclusion<  typename  detail::P_conjunction<p1, p2>::type>;

	template<class main_pack, class det>
	using p_detach = typename detail::P_detach< det, main_pack >::type;

	template<class num, size_t start, size_t end>
	using p_sub = typename detail::P_cut_first< typename detail::P_cut_last<num, end>::type, start>::type;

	template<class p1, class p2>
	using p_disjunction = p_exclusion<p_merge<p1, p2>>;
}



