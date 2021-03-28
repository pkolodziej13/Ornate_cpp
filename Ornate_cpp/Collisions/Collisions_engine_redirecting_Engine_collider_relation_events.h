#pragma once
#include "Collisions_requirements.h"
namespace col
{
	struct Engine_unified_relation_side
	{
		virtual void verify_relation_with_collider_after_add(Colider_unified& collider, Object_unified& object) = 0;
		virtual void verify_relation_with_collider_after_remove(Colider_unified& collider, Object_unified& object) = 0;
	}; 
	template<class Collecting_engine>
	struct Engine_collider_relation;
	template<class Individualist>
	struct Engine_relation_side :virtual Engine_unified_relation_side
	{
		virtual void verify_relation_with_collider_after_add(Colider_unified& collider, Object_unified& object)
		{
			Engine_collider_relation<Individualist>::verify_relation_after_add(static_cast<Individualist&>(*this), collider, object);
		}
		virtual void verify_relation_with_collider_after_remove(Colider_unified& collider, Object_unified& object)
		{
			Engine_collider_relation<Individualist>::verify_relation_after_remove(static_cast<Individualist&>(*this), collider, object);
		}
	};

}