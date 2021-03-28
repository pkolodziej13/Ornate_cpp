#include "pch.h"
#include "CppUnitTest.h"
#include <Aggregations/Pooled_indexes.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


TEST_CLASS(pool_test)
{
public:

	TEST_METHOD(indexes_adding_free)
	{
		agg::Pooled_indexes pool;
		Assert::AreEqual<size_t>(0u,pool.add());
		Assert::AreEqual<size_t>(1u, pool.add());
		Assert::AreEqual<size_t>(2u, pool.add());
		pool.free(1);
		Assert::AreEqual<size_t>(1u, pool.add());
	}

};