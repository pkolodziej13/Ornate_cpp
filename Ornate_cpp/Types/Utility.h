#pragma once
#include <type_traits>
#include <typeinfo>


namespace typ
{
	template<class t>
	using type_of = typename t::type;

	template<class t>
	struct Overtype
	{
		using type = t;
	};

	template<class base, class derivered>
	using is_base_or_same = std::integral_constant<bool, std::is_base_of_v<base, derivered> || std::is_same_v<base, derivered>>;
	template<class base, class derivered>
	constexpr bool is_base_or_same_v = is_base_or_same<base, derivered>::value;

	template< class t, class = void >
	struct is_type_existing :std::false_type {};
	template<class t>
	struct  is_type_existing<t, decltype(typeid(t), void())> :std::true_type
	{
	};
	template<class t>
	using type_existance_t = std::enable_if_t< is_type_existing<t>::value>;


	template <template <typename...> class C, typename...Ts>
	std::true_type is_template_base_of_impl(const C<Ts...>*);

	template <template <typename...> class C>
	std::false_type is_template_base_of_impl(...);

	template < template <typename...> class C, typename T>
	using is_template_base_of = decltype(is_template_base_of_impl<C>(std::declval<T*>()));


	namespace detail
	{
		template<class checked, class kind_trial>
		struct kind_recognizer
		{
			template<class t, class = decltype(kind_trial::kind_trial(std::declval<checked&>()))>
			static std::true_type type_trial(int);
			template<class t>
			static std::false_type type_trial(...);

			using type = decltype(type_trial<checked>(0));
			static constexpr bool value = type::value;
		};
	}

	template<class checked, class kind_trial>
	constexpr bool is_kind = detail::kind_recognizer<checked, kind_trial>::value;

	template<class checked,class kind_trial>
	using kind_of = decltype(kind_trial::kind_trial(std::declval<checked>()));


	template<class T1, class T2>
	struct Symetric
	{
		static constexpr bool use_that_order = is_type_existing<Symetric<T2, T1>>::value;
		using type1 = std::conditional_t<use_that_order,T1,T2>;
		using type2 = std::conditional_t<use_that_order, T2, T1>;
	};
	template< class T>
	struct Symetric<T, T>
	{
		using type1 = T;
		using type2 = T;
	};

	template<class T1, class T2>
	using Symetric1 = typename Symetric<T1,T2>::type1;
	template<class T1, class T2>
	using Symetric2 = typename Symetric<T1, T2>::type2;
}

#define s_c_expr static constexpr
#define decl_parameter(name) typ::type_of<std::decay_t<decltype(name)>>
#define decl_index(name) std::decay_t<decltype(name)>::value;