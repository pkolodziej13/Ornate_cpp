#pragma once
#include <type_traits>
#include <typeinfo>

namespace typ
{
	template<class T>
	using type_of = typename T::type;

	template<class T>
	struct Overtype
	{
		using type = T;
	};

	template<class Base, class Derivered>
	using is_base_or_same = std::integral_constant<bool, std::is_base_of_v<Base, Derivered> || std::is_same_v<Base, Derivered>>;
	template<class Base, class Derivered>
	constexpr bool is_base_or_same_v = is_base_or_same<Base, Derivered>::value;

	template< class T, class = void >
	struct is_type_existing :std::false_type {};
	template<class T>
	struct  is_type_existing<T, decltype(typeid(T), void())> :std::true_type
	{
	};
	template<class T>
	using type_existance_t = std::enable_if_t< is_type_existing<T>::value>;


	namespace detail
	{
		template <template <typename...> class Template_type, typename...T_v>
		std::true_type is_template_base_of_impl(const Template_type<T_v...>*);

		template <template <typename...> class Template_type>
		std::false_type is_template_base_of_impl(...);
	}

	template < template <typename...> class Template_type, typename T>
	using is_template_base_of = decltype(detail::is_template_base_of_impl<Template_type>(std::declval<T*>()));


	namespace detail
	{
		template<class Checked, class Kind_trial>
		struct kind_recognizer
		{
			template<class T, class = decltype(Kind_trial::kind_trial(std::declval<Checked&>()))>
			static std::true_type type_trial(int);
			template<class T>
			static std::false_type type_trial(...);

			using type = decltype(type_trial<Checked>(0));
			static constexpr bool value = type::value;
		};
	}

	template<class Checked, class Kind_trial>
	constexpr bool is_kind = detail::kind_recognizer<Checked, Kind_trial>::value;

	template<class Checked,class Kind_trial>
	using kind_of = decltype(Kind_trial::kind_trial(std::declval<Checked>()));


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