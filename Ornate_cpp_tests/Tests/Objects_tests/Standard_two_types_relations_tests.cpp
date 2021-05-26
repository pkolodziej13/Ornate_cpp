#include "pch.h"
#include "CppUnitTest.h"

#include <Objects/Relations_popular.h>

template<class rel1, class rel2, class Value >
void relation_test()
{
	struct D;
	struct E;
	using relation2 = obj::Relation_asymmetric<rel1, rel2, D, E, Value>;

	struct D :relation2::template Individual<0>
	{};
	struct E :relation2::template Individual<1>
	{};

	D d;
	E e;
	
	d.clear_relations();
	e.clear_relations();

	relation2::create_relation(d, e);

	assert(relation2::relation_exist(d, e));
	if constexpr (!std::is_same_v<Value, void>)
	{
		auto c = relation2::relation_value(d, e) = 10;
	}

	relation2::remove_relation(d, e);

	d.clear_relations();
	e.clear_relations();
}
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
template<class T1, class ... T2>
void test_for_relations()
{
	((relation_test<T1, T2, void>()), ...);
	((relation_test<T1, T2, int>()), ...);
}
template<class ... T1>
void test_for_multiple_relations()
{
	((test_for_relations<T1, T1...>()), ...);
}
TEST_CLASS(Objects_tests)
{
public:

	TEST_METHOD(standard_two_types_relations_test)
	{
		test_for_multiple_relations<obj::Relation_side_single, obj::Relation_side_map, obj::Relation_side_pool, obj::Relation_side_dense_list, obj::Relation_side_dense_indexed>();

	}
};