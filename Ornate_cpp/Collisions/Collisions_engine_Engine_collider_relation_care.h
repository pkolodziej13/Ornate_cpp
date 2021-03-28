#pragma once
namespace col
{
	struct Collider_object_relation_eventer
	{
		struct On_create
		{
			template<class Object_type, class Collider_type>
			bool before_create(Object_type& object, Collider_type& collider)
			{
				if (object.is_related_with_engine())
				{
					return collider.can_absorb_layer_in_engine(object, object.engine());
				}
				return true;
			}
			template<class Object_type, class Collider_type>
			void after_create(Object_type& object, Collider_type& collider)
			{
				if (object.is_related_with_engine())
				{
					object.engine().verify_relation_with_collider_after_add(collider, object);
				}
			}
		};
		struct On_remove
		{
			template<class Object_type, class Collider_type>
			On_remove(Object_type& object, Collider_type& collider)
			{
			}
			template<class Object_type, class Collider_type>
			void after_remove(Object_type& object, Collider_type& collider)
			{
				if (object.is_related_with_engine())
					object.engine().verify_relation_with_collider_after_remove(collider, object);
			}
		};
	};

	struct Object_engine_relation_eventer
	{
		struct On_create
		{
			template<class Engine_type, class Object_type>
			bool before_create(Engine_type& engine, Object_type& object)
			{
				bool result = true;
				object.for_every_related_collider(
					[&](Colider_unified& collider)
				{
					if (!collider.can_absorb_layer_in_engine(object, engine))
						result = false;
				});
				return result;
			}
			template<class Engine_type, class Object_type>
			void after_create(Engine_type& engine, Object_type& object)
			{
				object.for_every_related_collider(
					[&](Colider_unified& collider)
				{
					engine.verify_relation_with_collider_after_add(collider, object);
				});
			}
		};
		struct On_remove
		{
			template<class Engine_type, class Object_type>
			On_remove(Engine_type& engine, Object_type& object)
			{
				engine.remove_contacts_of(object);//do odzielenia
			}
			template<class Engine_type, class Object_type>
			void after_remove(Engine_type& engine, Object_type& object)
			{
				Engine_type* eptr = &engine;
				Object_type* optr = &object;
				object.for_every_related_collider(
					[&](Colider_unified& collider)
				{
					engine.verify_relation_with_collider_after_remove(collider, object);
				});
			}
		};
	};

}