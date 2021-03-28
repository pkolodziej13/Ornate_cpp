#pragma once
#include <algorithm>

#include "Grouping_in_synchronized_pool.h"
#include "Grouping_colliders_regard_layers.h"
#include "Collisions_engine_Engine_collider_relation.h"
namespace col
{
	template<class Individualist, class Comparer, template<size_t I>class Collider_rejestr>
	struct Rejestring_colliders_considering_objects_number:
		Engine_collider_relation_wing<Rejestring_colliders_considering_objects_number<Individualist, Comparer, Collider_rejestr>, Comparer>
	{
		using Layers_p = typename Comparer::Layers_p;
		using Layers_key = std::bitset<Layers_p::size>;

		template<class Collider_type>
		std::vector<Object_unified*> objects_of_collider(Collider_type& collider)
		{
			std::vector<Object_unified*> objects_of_engine;
			auto lam = [&](auto& object)
			{
				objects_of_engine.push_back(&object);
			};
			collider.for_every_object_conected_with_engine(individualist(), lam);
			std::sort(objects_of_engine.begin(), objects_of_engine.end(),
				[](Object_unified* object1, Object_unified* object2)
			{
				return object1->layer_index() > object2->layer_index();
			});
			return objects_of_engine;
		}

		template<class Collider_type>
		struct Access_info
		{
			Collider_type* collider;
			Layers_key layers_key;
			size_t position;
			std::vector<Object_unified*> connected_objects;
		};
		template<class Collider_type>
		size_t add_collider_on_this_side(Collider_type& collider, const Layers_key& key)
		{
			Access_info<Collider_type> access_info{ &collider ,key , 0,objects_of_collider(collider) };
			elements.add_element(key, access_info);
			return access_info.position;
		}
		template<class Collider_type>
		size_t change_collider_on_this_side(Collider_type& collider, size_t old_place, const Layers_key& old_key, const Layers_key& new_key)
		{
			Access_info<Collider_type> access_info{ &collider ,old_key , old_place, objects_of_collider(collider) };
			elements.remove_element(old_key, access_info);
			access_info.layers_key = new_key;
			elements.add_element(new_key, access_info);
			return access_info.position;
		}
		template<class Collider_type>
		void remove_collider_on_this_side(Collider_type& collider, size_t old_place, const Layers_key& old_key)
		{
			Access_info<Collider_type> access_info{ &collider ,old_key , old_place,{} };
			elements.remove_element(old_key, access_info);
		}
		void synchronize()
		{
			elements.for_each([&](Layers_key key, auto& segregator)
			{
				segregator.synchronize();
				segregator.for_each([](auto& rejestr)
				{
					rejestr.synchronize();
				});
			}
			);
		}
		template<class Lambda>
		void for_each_collider_rejestr(Lambda&& lambda)
		{
			elements.for_each([&](auto key, auto& element)
			{
				element.for_each([&](auto& segregator_element)
				{
					lambda(segregator_element);
				});
			});
		}
		template<class Lambda>
		void evaluate_colliders(Lambda&& lambda)
		{
			elements.evaluate_colliders_segregator([&](auto key1, auto key2, auto& colliders1, auto& colliders2)
			{
				auto keys_connections = Contacts_transfering<Comparer>::get_connections_of_keys(key1, key2);
				if (key1 == key2)
					colliders1.evaluate_self([&](auto& collider1, auto& collider2)
				{
					lambda(keys_connections, collider1, collider2);
				});
				else
					colliders1.evaluate_with(colliders2, [&](auto& collider1, auto& collider2)
				{
					lambda(keys_connections, collider1, collider2);
				});
			});
		}
		inline Individualist& individualist()
		{
			return static_cast<Individualist&>(*this);
		}

		template<size_t I>
		using make_segregator = Grouping_in_synchronized_pool<Collider_rejestr<I>>;
		Grouping_colliders_regard_layers<Comparer, make_segregator> elements;
	};
}