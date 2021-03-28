#include "pch.h"
#include "CppUnitTest.h"

#include <Types/utility.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(utility_types_tests)
{
public:
	template<class t, class t2, size_t I>
	struct B {};

	struct B_kind
	{
		template<class t, class t2, size_t I>
		static void kind_trial(B<t, t2, I>&);
	};
	TEST_METHOD(kind_recognising)
	{
		static_assert(typ::is_kind<B<int, int, 9>, B_kind>);
		static_assert(!typ::is_kind<int, B_kind>);
	}
};