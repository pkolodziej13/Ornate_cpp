#pragma once
#include <type_traits>

namespace std::experimental//remove when appear in standard
{
	struct nonesuch 
	{
		nonesuch() = delete;
		~nonesuch() = delete;
		nonesuch(nonesuch const&) = delete;
		void operator=(nonesuch const&) = delete;
	};
	namespace detail 
	{
		template <class Default, class AlwaysVoid, template<class...> class Op, class... Args>
		struct detector 
		{
			using value_t = std::false_type;
			using type = Default;
		};

		template <class Default, template<class...> class Op, class... Args>
		struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> 
		{
			using value_t = std::true_type;
			using type = Op<Args...>;
		};

	} 

	template <template<class...> class Op, class... Args>
	using is_detected = typename detail::detector<nonesuch, void, Op, Args...>::value_t;

	template <template<class...> class Op, class... Args>
	using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;

	template <class Default, template<class...> class Op, class... Args>
	using detected_or = detail::detector<Default, void, Op, Args...>;
}

#define has_member_function(member)    \
	template<typename T, typename = void> struct Has_##member : std::false_type { }; \
	template<typename T>                  struct Has_##member<T, decltype(T::##member, void())> : std::true_type {  }; \
	template<class t> constexpr bool has_##member##_v  = Has_##member<t>::value;

#define has_member_type(member)    \
	template<class t> using member##_of = typename t::##member;\
	template<class t>  struct has_##member :std::experimental::is_detected<member##_of, t> {};

#define has_member_template(member)    \
	template<class t,class t2> using member##_of = typename t::template member##<t2>;\
	template<class t>  struct has_##member :std::experimental::is_detected<member##_of, t,int> {};

namespace typ
{
	namespace detail
	{
		template<class Base, template<class >class Trail_template>
		struct Detector
		{
			template<class T, class = Trail_template<T>>
			static std::true_type trail(int);
			template<class t>
			static std::false_type trail(...);

			using type = decltype(trail<Base>(0));
			static constexpr bool value = type::value;
		};

	}

	template <class Base, template<class >class Trail_template>
	using is_detected_t = typename detail::Detector<Base, Trail_template>::type;
	template <class Base, template<class >class Trail_template>
	constexpr bool is_detected = is_detected_t<Base, Trail_template>::value;

}