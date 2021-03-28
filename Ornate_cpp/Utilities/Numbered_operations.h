#pragma once
#include "Numeration.h"
#include "loops.h"
namespace uti
{
	template<class ... tt>
	auto tie_numeration(tt &... args)
	{
		return Numeration<tt & ...>(args...);
	}
	template<class ... tt>
	auto make_numeration(tt ... args)
	{
		return Numeration<std::decay_t<tt>...>(args...);
	}

	template<class ... t_v, class t>
	Numeration<t_v..., t> add_to_numeration(const Numeration<t_v...>& num, const t& to_add)
	{
		return num.expand([&](auto &&... elems) {return Numeration<t_v..., t>{elems..., to_add}; });
	}
	template< class t, class ... t_v>
	Numeration< t, t_v...> add_to_numeration(const t& to_add, const Numeration<t_v...>& num)
	{
		return num.expand([&](auto &&... elems) {return Numeration< t, t_v...>{to_add, elems...}; });
	}
	template<size_t i, class ... t_v>
	auto remove_from_numeration(const Numeration<t_v...>& num)
	{
		using indexes_p = typ::p_index_sequence<sizeof...(t_v)>;
		using cleaned_p = typ::p_delete_at<i, indexes_p>;
		using result_type = typ::p_transform<Numeration, typ::p_delete_at<i, typ::Pack<t_v...>>>;
		return cleaned_p::expand([&](auto ... args)
		{
			return result_type{ num.get<typ::type_of<decltype(args)>::value>()... };
		});
	}

	template<class N, size_t end>
	using n_cut_at = typ::p_transform<Numeration, typ::p_sub<typ::p_from<N>, 0, end>>;

	namespace detail
	{
		template<class N, class too_add>
		struct N_collapse;
		template<class ...nn, class  n2>
		struct N_collapse<Numeration<nn...>, n2>
		{
			using type = Numeration<nn..., n2>;
		};
		template<class ...nn, class n1, class ... nn2>
		struct N_collapse< Numeration<nn...>, Numeration<n1, nn2...>>
		{
			using leading_numbered = typename N_collapse <Numeration<nn...>, n1>::type;
			using type = typename N_collapse<leading_numbered, Numeration<nn2...>>::type;
		};
		template<class ...nn>
		struct N_collapse< Numeration<nn...>, Numeration<>>
		{
			using type = Numeration<nn...>;
		};

		template<class ...nn>
		using n_collapse = typename N_collapse<Numeration<>, Numeration<nn...>>::type;

		template<size_t passed, class ... tn, class n>
		auto n_Collapse_detail(Numeration<tn...>& target, const n& to_collapse)
		{
			target.get<passed>() = to_collapse;
		}
		template<size_t passed, class ... tn, class ... nn2>
		auto n_Collapse_detail(Numeration<tn...>& target, const Numeration<nn2...>& to_collapse)
		{
			uti::for_index_sequence<sizeof...(nn2)>([&](auto a)
			{
				constexpr size_t i = decltype(a)::value;
				constexpr size_t progress = n_collapse<n_cut_at<Numeration<nn2...>, i>>::size;

				n_Collapse_detail<passed + progress>(target, to_collapse.get<i>());
			}
			);
		}
	}
	template<class ... nn>
	auto n_Collapse(const nn & ... args)
	{
		using target_type = detail::n_collapse<nn...>;
		target_type to_return;

		detail::n_Collapse_detail<0>(to_return, tie_numeration(args...));

		return to_return;

	}
}