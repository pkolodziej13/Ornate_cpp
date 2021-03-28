#include "pch.h"
#include "CppUnitTest.h"

#include <Objects/prototype.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


TEST_CLASS(prototype_test)
{
public:
	struct A
	{
		virtual int f() { return 0; }
	};
	struct B:A
	{
		virtual int f() 
		{ 
			return 1;
		}
	};

	TEST_METHOD(prototype_copying)
	{
		obj::Prototype<A> p1(B{});
		obj::Prototype<A> p2{p1};

		Assert::AreEqual(p1->f(),1 );
		Assert::AreEqual(p2->f(), 1);

		Assert::IsFalse(&p1.get()==&p2.get());
	}
	TEST_METHOD(prototype_basic_type_test)
	{
		obj::Prototype<int> i(0);
		Assert::AreEqual(i.get(), 0);
	}

};