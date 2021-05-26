#include "pch.h"
#include "CppUnitTest.h"

#include <Aggregations/Variations_callbacks.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
	
TEST_CLASS(Loose_list_test)
{
public:
	TEST_METHOD(invoke_steal)
	{
		agg::Invoker<> inv1;
		agg::Invoker<> inv2;
		int i = 0;
		auto a = inv2.add([&]() {i++; });
		inv1.steal_from(inv2);
		inv1.invoke();
		Assert::AreEqual(i,1);
	}
	TEST_METHOD(invoking_variation_dealyed_add)
	{
		agg::Variations_callbacks<typ::Pack<>, typ::Pack<int>, typ::Pack<>> variations;

		bool first_variation_control= false;
		auto a1 = variations.add_callback([&](int) 
		{
			first_variation_control= true; 
		});
		bool second_variation_control = false;
		auto a2 = variations.add_callback([&]() 
		{
			second_variation_control = true; 
		},0);
		variations.applay_added();
		variations.invoke(0);

		Assert::IsTrue(first_variation_control);
		Assert::IsTrue(second_variation_control);
	}

};