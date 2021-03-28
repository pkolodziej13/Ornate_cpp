#include "pch.h"
#include "CppUnitTest.h"

#include <Types/Members.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(utility_types_tests)
{
public:

	template<class t>
	using member_type_of = typename t::member_type;
	template<class t>
	using member_type2_of = typename t::member_type2;

	template<class t>
	using member_function_of = std::enable_if_t<std::is_same_v<void, decltype(t::member_function())>>;
	template<class t>
	using member_function2_of = std::enable_if_t<std::is_same_v<void, decltype(t::member_function2())>>;

	template<class t>
	using member_method_of = decltype(std::declval<t&>().member_method());
	template<class t>
	using member_method2_of = decltype(std::declval<t&>().member_method2());

	template<class t>
	using member_variable_of = decltype(std::declval<t&>().member_variable);
	template<class t>
	using member_variable2_of = decltype(std::declval<t&>().member_variable2);
	template<class t>
	using member_variable_private_of = decltype(std::declval<t&>().member_variable_private);

	struct A
	{
		using member_type = int;

		static void member_function();

		void member_method();

		int member_variable;
	private:
		int member_variable_private;

	};
	TEST_METHOD(members_detecting)
	{
		static_assert(typ::is_detected<A, member_type_of>);
		static_assert(!typ::is_detected<A, member_type2_of>);

		static_assert(typ::is_detected<A, member_function_of>);
		static_assert(!typ::is_detected<A, member_function2_of>);

		static_assert(typ::is_detected<A, member_method_of>);
		static_assert(!typ::is_detected<A, member_method2_of>);

		static_assert(typ::is_detected<A, member_variable_of>);
		static_assert(!typ::is_detected<A, member_variable2_of>);
		static_assert(!typ::is_detected<A, member_variable_private_of>);
	}
};