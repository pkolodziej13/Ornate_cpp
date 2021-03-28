#pragma once
#include <Objects/Relation_final.h>
#include <Objects/Relations_popular.h>
#include <Objects/Standard_relations.h>

#include "Collisions_requirements.h"
#include "Collisions_engine_Engine_collider_relation_care.h"
#include "Engine_object_relation.h"
#include "Object_contacts.h"
#include "Object_side_engine_relation.h"

namespace col
{
	using Relation_object_colider = obj::Relation_profiled_symetric_events<Collider_object_relation_eventer, obj::Dense_indexed_relation, obj::Dense_list_relation, Object_unified, Colider_unified>;
	
	template<class Layer, class Engine, class Comarer>
	using Relation_engine_object =
		obj::Relation_final<obj::Relation_with_events<Object_engine_relation_eventer, Engine_object_relation<Engine, Layer, Object_contacts<Layer, Comarer> >>>;
}