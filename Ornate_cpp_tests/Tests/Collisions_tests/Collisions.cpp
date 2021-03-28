#include "pch.h"
#include "CppUnitTest.h"

#include <Collisions\Collisions_engine_complete.h>
#include <Collisions\Box.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Collisions_engine_tests)
{
public:

	struct A :col::Object_individual<A>
	{
		using collidable = typ::Pack<A>;

		col::Collider<col::Ball> collider;
		A()
		{
			col::Relation_object_colider::create_relation(*this, collider);
		}
		static void on_enter_same_collision(A& a, A& b)
		{}
		static void during_same_collision(A& a, A& b)
		{}
		static void on_exit_same_collision(A & a, A & b)
		{}
	};
	TEST_METHOD(Creating_connnecting_and_destroying_stability)
	{
		A a1;
		A a2;
		{
			col::Engine_braid<typ::Pack<A>, typ::Pack<col::Ball>> engine;
			engine.relate_with_object(a1);
			engine.relate_with_object(a2);

			engine.update_collisions_state();
		}
	}
	TEST_METHOD(Destroying_connected_object_stability)
	{
		col::Engine_braid<typ::Pack<A>, typ::Pack<col::Ball>> engine;
		{
			A a1;
			engine.relate_with_object(a1);
		}
	}
	TEST_METHOD(Destroying_connected_collider_stability)
	{
		col::Engine_braid<typ::Pack<A>, typ::Pack<col::Ball>> engine;
		A a1;
		A a2;
		{
			col::Collider<col::Ball> additional_ball;
			a1.relate_with_collider(additional_ball);
			engine.relate_with_object(a1);
			engine.relate_with_object(a2);
			engine.update_collisions_state();
		}
	}

	struct B :col::Object_individual<B>
	{
		using collidable = typ::Pack<B>;

		bool entered_collison = false;
		bool exited_collision = false;
		static void on_enter_same_collision(B& a, B& b)
		{
			a.entered_collison = true;
			b.entered_collison = true;
		}
		static void during_same_collision(B& a, B& b)
		{}
		static void on_exit_same_collision(B& a, B& b)
		{
			a.exited_collision = true;
			b.exited_collision = true;
		}
	};

	TEST_METHOD(Using_multiple_colliders_types)
	{
		col::Engine_braid<typ::Pack<B>, typ::Pack<col::Ball,col::Box>> engine;
		B a;
		col::Collider<col::Ball> ball;
		a.relate_with_collider(ball);

		B b;
		col::Collider<col::Box> box;
		box.current.coordines_as_matrix.translate({ 10,0,0 });
		b.relate_with_collider(box);

		engine.relate_with_object(a);
		engine.relate_with_object(b);
		engine.update_collisions_state();

		Assert::IsFalse(a.entered_collison);
		Assert::IsFalse(b.entered_collison);

		box.current.coordines_as_matrix.translate({ -10,0,0 });
		engine.update_collisions_state();

		Assert::IsTrue(a.entered_collison);
		Assert::IsTrue(b.entered_collison);
	}

	TEST_METHOD(On_collision_exit)
	{
		col::Engine_braid<typ::Pack<B>, typ::Pack<col::Ball>> engine;
		B a;
		col::Collider<col::Ball> ball1;
		a.relate_with_collider(ball1);

		B b;
		col::Collider<col::Ball> ball2;
		b.relate_with_collider(ball2);

		engine.relate_with_object(a);
		engine.relate_with_object(b);
		engine.update_collisions_state();

		Assert::IsTrue(a.entered_collison);
		Assert::IsTrue(b.entered_collison);

		ball2.current.center += mth::vec3(10, 0, 0);
		engine.update_collisions_state();

		Assert::IsTrue(a.exited_collision);
		Assert::IsTrue(b.exited_collision);
	}

	TEST_METHOD(Using_multiple_objects_types)
	{
		struct C :col::Object_individual<C>
		{
			using collidable = typ::Pack<>;
		};
		struct D :col::Object_individual<D>
		{
			using collidable = typ::Pack<C>;

			bool entered_collison = false;
			bool exited_collision = false;
			void on_enter_collision(C& c)
			{
				entered_collison = true;
			}
			void during_collision(C& c)
			{}
			void on_exit_collision(C& c)
			{
				exited_collision = true;
			}
		};

		col::Engine_braid<typ::Pack<C,D>, typ::Pack<col::Ball>> engine;
		C c;
		col::Collider<col::Ball> ball1;
		c.relate_with_collider(ball1);

		D d;
		col::Collider<col::Ball> ball2;
		d.relate_with_collider(ball2);

		engine.relate_with_object(c);
		engine.relate_with_object(d);
		engine.update_collisions_state();

		Assert::IsTrue(d.entered_collison);

		ball2.current.center += mth::vec3(10, 0, 0);
		engine.update_collisions_state();

		Assert::IsTrue(d.exited_collision);
	}

	TEST_METHOD(Bilateral_colisions_detecting)
	{
		struct D;
		struct C :col::Object_individual<C>
		{
			using collidable = typ::Pack<D>;

			bool entered_collison = false;
			bool exited_collision = false;
			void on_enter_collision(D& c)
			{
				entered_collison = true;
			}
			void during_collision(D& c)
			{}
			void on_exit_collision(D& c)
			{
				exited_collision = true;
			}
		};
		struct D :col::Object_individual<D>
		{
			using collidable = typ::Pack<C>;

			bool entered_collison = false;
			bool exited_collision = false;
			void on_enter_collision(C& c)
			{
				entered_collison = true;
			}
			void during_collision(C& c)
			{}
			void on_exit_collision(C& c)
			{
				exited_collision = true;
			}
		};

		col::Engine_braid<typ::Pack<C, D>, typ::Pack<col::Ball>> engine;
		C c;
		col::Collider<col::Ball> ball1;
		c.relate_with_collider(ball1);

		D d;
		col::Collider<col::Ball> ball2;
		d.relate_with_collider(ball2);

		engine.relate_with_object(c);
		engine.relate_with_object(d);
		engine.update_collisions_state();

		Assert::IsTrue(c.entered_collison);
		Assert::IsTrue(d.entered_collison);

		ball2.current.center += mth::vec3(10, 0, 0);
		engine.update_collisions_state();

		Assert::IsTrue(c.exited_collision);
		Assert::IsTrue(d.exited_collision);
	}
};