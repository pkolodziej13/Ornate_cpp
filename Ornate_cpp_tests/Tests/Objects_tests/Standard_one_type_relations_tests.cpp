#include "pch.h"
#include "CppUnitTest.h"

#include <Objects/Relations_popular.h>

template<class rel1, class rel2, class Value  >
void relation_test_with_single_type()
{
	struct A;
	using relation2 = obj::Relation_profiled<obj::Relation_asymmetric<rel1, rel2, A, A, Value>>;

	struct A :relation2::template Individual_of<A>
	{};

	A a;
	A b;


	relation2::create_relation(a, b);


	assert(relation2::relation_exist(a, b));
	if constexpr (!std::is_same_v<Value, void>)
	{
		auto c = relation2::relation_value(a, b) = 10;
	}

	relation2::remove_relation(a, b);
}
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
template<class T1, class ... T2>
void test_for_relations()
{
	((relation_test_with_single_type<T1, T2, void>()), ...);
	((relation_test_with_single_type<T1, T2, int>()), ...);
}
template<class ... T1>
void test_for_multiple_relations()
{
	((test_for_relations<T1, T1...>()), ...);
}
TEST_CLASS(Objects_tests)
{
public:

	TEST_METHOD(standard_one_types_relations_test)
	{
		test_for_multiple_relations<obj::Relation_side_single, obj::Relation_side_map, obj::Relation_side_pool, obj::Relation_side_dense_list, obj::Relation_side_dense_indexed>();

	}
};