#include "pch.h"
#include "CppUnitTest.h"

#include <array>

#include <Aggregations/Loose_List.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Loose_list_test)
{
public:

	TEST_METHOD(Fast_iteration)
	{
		agg::Loose_list<int> list;

		auto a1 = list.create_as_last(1);
		auto a2 = list.create_as_last(2);
		auto a3 = list.create_as_last(3);
		auto a4 = list.create_as_last(4);

		int it = 1;
		for (auto& a : list.fast_range())
		{
			Assert::AreEqual(a, it);
			it++;
		}
	}
	TEST_METHOD(destruction)
	{
		agg::Loose_list<int> list;

		auto a1 = list.create_as_last(1);
		auto a2 = list.create_as_last(2);
		{
			auto a3 = list.create_as_last(3);
		}
		auto a4 = list.create_as_last(4);

		int it = 0;
		std::vector<int> expected{ 1,2,4 };
		for (auto& a : list.save_range())
		{
			Assert::AreEqual(a, expected[it]);
			it++;
		}
	}
	TEST_METHOD(Save_iteration)
	{
		agg::Loose_list<int> list;

		auto a1 = list.create_as_last(1);
		auto a2 = list.create_as_last(2);
		auto a3 = list.create_as_last(3);
		auto a4 = list.create_as_last(4);

		int it = 1;
		for (auto& a : list.save_range())
		{
			Assert::AreEqual(a, it);
			it++;
		}
	}
	TEST_METHOD(Removing_during_save_loop)
	{
		agg::Loose_list<int> list;

		auto a1 = list.create_as_last(1);
		auto a2 = list.create_as_last(2);
		auto a3 = list.create_as_last(3);

		int i = 0;
		for (auto& a : list.save_range())
		{
			if (i == 0)
			{
				a1.reset();
			}
			else if (i == 1)
			{
				Assert::AreEqual(a, 2);
				a2.reset();
				a3.reset();
			}
			else if (i > 1)
			{
				Assert::Fail(L"Loop cannot reach this scope");
			}
			i++;
		}
	}
	TEST_METHOD(attach_after)
	{
		agg::Loose_list<int> list;

		auto a1 = list.create_as_last(1);
		auto a2 = list.create_as_last(2);
		auto a3 = list.create_as_last(3);
		auto a4 = agg::Loose_element<int>(4);
		auto a5 = std::make_shared<agg::Loose_element<int> >(5);
		a4.link_chain_as_previous(*a5);
		list.link_chain_as_next(a4);

		int it = 0;
		std::array<int, 5> expected{ 4,5,1,2,3 };
		for (auto& a : list.save_range())
		{
			Assert::AreEqual(a, expected[it]);
			it++;
		}
	}
	TEST_METHOD(attach_before)
	{
		agg::Loose_list<int> list;

		auto a1 = list.create_as_last(1);
		auto a2 = list.create_as_last(2);
		auto a3 = list.create_as_last(3);
		auto a4 = agg::Loose_element<int>(4);
		auto a5 = std::make_shared<agg::Loose_element<int> >(5);
		a4.link_chain_as_previous(*a5);
		list.link_chain_as_previous(a4);

		int it = 0;
		std::array<int, 5> expected{ 1,2,3, 4,5 };
		for (auto& a : list.save_range())
		{
			Assert::AreEqual(a, expected[it]);
			it++;
		}
	}
};