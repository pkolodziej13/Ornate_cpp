#pragma once
#include "Collisions_engine_relations.h"
#include "Collisions_engine_matching_colliders_with_objects.h"
#include "Collisions_engine_Engine_collider_relation.h"
#include "Collisions_engine_rejestring_colliders_considering_objects_number.h"

namespace col
{
	struct Colider_unified :
		protected Relation_object_colider::Individual<1>,
		private Collider_layers_checking<Colider_unified>,
		private Collider_relation_side<Colider_unified>
	{
		friend Relation_object_colider::Wing_conversion;
		friend Relation_object_colider;
		friend Collider_object_relation_eventer;
		friend Object_engine_relation_eventer;
		friend Engine_collider_relation;
		friend Engine_collider_relation_wing;
		friend Rejestring_colliders_considering_objects_number;
		friend Collider_relation_side<Colider_unified>;
		friend Collider_layers_checking<Colider_unified>;
		template<class Lambda>
		void for_every_object(Lambda&& lambda)
		{
			Relation_object_colider::for_each_related<1>(*this, lambda);
		}
		void clear_objects_relations()
		{
			this->clear_relations();
		}

		virtual ~Colider_unified() = default;
	};
	template<class T>
	struct Collider :Colider_unified
	{
		T current;
		~Collider()
		{
			this->clear_relations();
		}
	};
}