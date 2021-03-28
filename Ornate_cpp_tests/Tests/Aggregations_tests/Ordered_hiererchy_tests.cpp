#include "pch.h"
#include "CppUnitTest.h"

#include <Aggregations/Ordered_tree_view.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


TEST_CLASS(Ordered_hiererchy_test)
{
public:

	TEST_METHOD(adding_in_constructor)
	{
		agg::Ordered_tree<int > tree;
		auto & child1 = tree.add_child();
		auto & child2 = tree.add_child();
		auto & child3 = child1.add_child();
		tree.get() = 1;
		child1.get() = 2;
		child3.get() = 3;
		child2.get() = 4;


		auto elements = tree.all_childs_objects();

		Assert::AreEqual(1, elements[0].get());
		Assert::AreEqual(2, elements[1].get());
		Assert::AreEqual(3, elements[2].get());
		Assert::AreEqual(4, elements[3].get());
	}

	struct A
	{
		int a = 0;
		virtual void f() = 0;
	};
	struct B :A 
	{
		virtual void f() {}
	};
	struct C :B {};

	TEST_METHOD(sugested_node)
	{

		agg::Ordered_tree<C > tree;
		agg::Ordered_tree_view<B&> prox( tree );
		prox->a = 1;

		Assert::AreEqual(tree->a, 1);

		agg::Ordered_tree_view<A&> prox2(prox);
		prox2->a = 2;

		Assert::AreEqual(tree->a, 2);

	}
	TEST_METHOD(sugested_node_childs)
	{

		agg::Ordered_tree<C > tree;
		agg::Ordered_tree_view<B&> prox(tree);
		agg::Ordered_tree_view<B&> added_prox = prox.add_child();
		added_prox->a = 1;
		agg::Ordered_tree_view<A&> prox2(added_prox);
		agg::Ordered_tree_view<A&> added_prox2 = prox2.add_child();
		added_prox2->a = 2;


		const auto & node1 = tree.get_childs().front();
		Assert::AreEqual(node1->a, 1);
		const auto & node2 = node1.get_childs().front();
		Assert::AreEqual(node2->a, 2);

	}
	TEST_METHOD(hierarchy_loop)
	{

		agg::Ordered_tree<int > tree;
		tree.get() = 0;
		auto & n1 = tree.add_child();
		n1.get() = 1;
		auto & n2 = tree.add_child();
		n2.get() = 2;
		auto & n3 = n1.add_child();
		n3.get() = 3;

		std::vector<int> expected{ 0,1,3,2 };
		int i = 0;
		for (auto & a : tree.range())
		{
			Assert::AreEqual(expected[i],a);
			i++;
		}

	}

};