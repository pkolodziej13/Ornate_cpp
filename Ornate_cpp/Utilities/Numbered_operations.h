#pragma once
#include "Numeration.h"
#include "loops.h"

namespace uti
{
	template<class ... T_v>
	auto tie_numeration(T_v &... args_v)
	{
		return Numeration<T_v & ...>(args_v...);
	}
	template<class ... T_v>
	auto make_numeration(T_v const & ... args_v)
	{
		return Numeration<std::decay_t<T_v>...>(args_v...);
	}

	template<class ... T_v, class T>
	Numeration<T_v..., T> add_to_numeration(const Numeration<T_v...>& num, const T& to_add)
	{
		return num.expand([&](auto &&... elems) {return Numeration<T_v..., T>{elems..., to_add}; });
	}
	template< class T, class ... T_v>
	Numeration< T, T_v...> add_to_numeration(const T & to_add, const Numeration<T_v...>& num)
	{
		return num.expand([&](auto &&... elems) {return Numeration< T, T_v...>{to_add, elems...}; });
	}
	template<size_t i, class ... T_v>
	auto remove_from_numeration(const Numeration<T_v...>& num)
	{
		using indexes_p = typ::p_index_sequence<sizeof...(T_v)>;
		using cleaned_p = typ::p_delete_at<i, indexes_p>;
		using result_type = typ::p_transform<Numeration, typ::p_delete_at<i, typ::Pack<T_v...>>>;
		return cleaned_p::expand([&](auto ... args)
		{
			return result_type{ num.get<typ::type_of<decltype(args)>::value>()... };
		});
	}


	namespace detail
	{
		template<class N, size_t end>
		using n_cut_at = typ::p_transform<Numeration, typ::p_sub<typ::p_from<N>, 0, end>>;

		template<class Base_numeration, class To_add>
		struct N_collapse;
		template<class ...T_v, class  To_add>
		struct N_collapse<Numeration<T_v...>, To_add>
		{
			using type = Numeration<T_v..., To_add>;
		};
		template<class ...T1_v, class T, class ... T2_v>
		struct N_collapse< Numeration<T1_v...>, Numeration<T, T2_v...>>
		{
			using leading_numbered = typename N_collapse <Numeration<T1_v...>, T>::type;
			using type = typename N_collapse<leading_numbered, Numeration<T2_v...>>::type;
		};
		template<class ...T_v>
		struct N_collapse< Numeration<T_v...>, Numeration<>>
		{
			using type = Numeration<T_v...>;
		};

		template<class ...T_v>
		using n_collapse = typename N_collapse<Numeration<>, Numeration<T_v...>>::type;

		template<size_t passed, class ... T_v, class To_add>
		auto n_collapse_detail(Numeration<T_v...>& target, const To_add& to_collapse)
		{
			target.get<passed>() = to_collapse;
		}
		template<size_t passed, class ... T1_v, class ... T2_v>
		auto n_collapse_detail(Numeration<T1_v...>& target, const Numeration<T2_v...>& to_collapse)
		{
			uti::for_index_sequence<sizeof...(T2_v)>([&](auto a)
			{
				constexpr size_t i = decltype(a)::value;
				constexpr size_t progress = n_collapse<n_cut_at<Numeration<T2_v...>, i>>::size;

				n_collapse_detail<passed + progress>(target, to_collapse.get<i>());
			}
			);
		}
	}
	template<class ... T_v>
	auto n_Collapse(const T_v & ... args)
	{
		using Target_type = detail::n_collapse<T_v...>;
		Target_type to_return;

		detail::n_collapse_detail<0>(to_return, tie_numeration(args...));

		return to_return;
	}
}