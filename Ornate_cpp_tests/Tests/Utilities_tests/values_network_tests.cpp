#include "pch.h"
#include "CppUnitTest.h"

#include <Utilities/Values_cell_deduction.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


TEST_CLASS(values_network_test)
{
public:

	TEST_METHOD(provider_and_receptor_test)
	{
		uti::Free_provider<int> provider{ 0 };
		struct con_rep :uti::Receptor<int>
		{
			using uti::Receptor<int>::Receptor;
			virtual void proceed()
			{
				a = this->value();
			}
			int a;
		};
		con_rep receptor{ provider };

		provider.set(5);
		Assert::AreEqual(5, receptor.a);
	}
	TEST_METHOD(computing_cell_test)
	{
		uti::Free_provider<int> provider1{ 3 };
		uti::Free_provider<float> provider2{ 2.f };
		int a;
		uti::Computing_cell<typ::Pack<int, float, int>, typ::Pack<>> cc
		{
			[&](int d,float b,int c)
			{
				a = int((d + b)*c);
			},
			provider1,provider2,provider1 
		};

		Assert::AreEqual(15, a);
		provider1.set(5);
		Assert::AreEqual(35, a);
		provider2.set(5);
		Assert::AreEqual(50, a);

	}
	TEST_METHOD(making_cell_test)
	{
		uti::Free_provider<int> p1{ 3 };
		uti::Free_provider<float> p2{ 2.f };

		int c = 0;
		auto cell = uti::make_computing_cell([&](int a, float b) {
			c = int( a + b );
		},p1, p2);

		Assert::AreEqual(5, c);
		p1.set(5);
		Assert::AreEqual(7, c);
	}
	TEST_METHOD(making_cell_from_cell_test)
	{
		uti::Free_provider<int> p1{ 1 };
		uti::Free_provider<float> p2{ 1.f };

		auto cell = uti::make_computing_cell([&](int a, float b,int & c,float &d) {
			c = int( a + b );
			d = a * b;
		}, p1, p2);

		int result = 0;
		auto cell2 = uti::make_computing_cell([&](int a, float b,int c) {
			result = int(a + b+c);
		}, cell, p1);

		Assert::AreEqual(4, result);
		p1.set(2);
		Assert::AreEqual(7, result);
	}

};