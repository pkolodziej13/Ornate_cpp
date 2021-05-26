#include "pch.h"
#include "CppUnitTest.h"
#include <Utilities/containing_shared_ptr.h>
#include <Utilities/holder_ptr.h>
#include <Utilities/containing_holder_ptr.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(pointers_tests)
{
public:

	TEST_METHOD(containing_shared_initialization)
	{
		uti::containing_shared_ptr <int> i = std::make_shared<int>(0);
		Assert::AreEqual(i.get(), 0);
	}

	TEST_METHOD(holder_from_shared)
	{
		std::shared_ptr<int> b;
		uti::holder_ptr<int> i = std::make_shared<int>(0);
		Assert::AreEqual(*i, 0);
	}
	TEST_METHOD(containing_holder_from_shared)
	{
		uti::containing_holder_ptr<int> i = std::make_shared<int>(0);
		Assert::AreEqual(i.get(),0);
	}

	TEST_METHOD(ontaining_shared_conversion)
	{
		struct A
		{};
		struct B :A
		{};
		uti::containing_shared_ptr<B> b = std::make_shared<B>();
		uti::containing_shared_ptr<A> a(b);
		Assert::IsTrue(&b.get()== &a.get());
	}
	TEST_METHOD(containing_shared_from_std_shared)
	{
		struct A
		{};
		struct B :A
		{};
		auto shared = std::make_shared<B>();
		uti::containing_shared_ptr<A> containing(shared);
		Assert::IsTrue(shared.get() == &containing.get());
	}

};