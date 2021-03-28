#pragma once
#include <utility>

namespace uti
{
	struct default_constructor
	{
	};
	template<class ... bases_v>
	struct variadic_inheritance : bases_v...
	{};
	template<class ... bases_v>
	struct variadic_virtual_inheritance : virtual bases_v...
	{};

	template<bool comparasion_result>
	struct Comparasion_in_constexpr
	{
		static constexpr bool true_in_constexpr = comparasion_result;
		static constexpr bool false_in_constexpr = !comparasion_result;
		static constexpr bool value = comparasion_result;
	};
	struct Comparasion_in_runtime
	{
		static constexpr bool true_in_constexpr = false;
		static constexpr bool false_in_constexpr = false;
		bool value{ false };
	};

	template<class Lambda>
	using Invoke_result = decltype((std::declval<Lambda>())());

	template<class t>
	struct conversion_checking
	{
		template<class t2>
		using type = std::is_convertible<t2, t>;
	};
}

