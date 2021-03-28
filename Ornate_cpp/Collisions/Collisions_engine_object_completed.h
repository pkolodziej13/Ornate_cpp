#pragma once
#include "Collisions_engine_relations.h"
#include "Collisions_engine_matching_colliders_with_objects.h"

namespace col
{
	struct Object_unified :
		Relation_object_colider::Individual<0>,
		Object_layer_identyfying
	{
		friend Relation_object_colider::Wing_conversion;
		friend Relation_object_colider;

		virtual bool is_related_with_engine() = 0;
		virtual Engine_unified& engine() = 0;
		template<class Lambda>
		void for_every_related_collider(Lambda&& lambda)
		{
			Relation_object_colider::for_each_related<0>(*this, lambda);
		}
	};
	template<class T>
	struct Object_individual :
		Object_unified,
		Object_side_engine_relation<Object_individual<T>, Engine<T>>::Object_individual,
		Object_layer_identifying_by_engine<Object_individual<T>>
	{
		using Layer = T;
		friend Object_layer_identifying_by_engine<Object_individual<T>>;
		friend Engine_object_relation;
		void relate_with_collider(Colider_unified& collider)
		{
			Relation_object_colider::create_relation(*this, collider);
		}
		void unrelate_collider(Colider_unified& collider)
		{
			Relation_object_colider::remove_relation(*this, collider);
		}
		void unrelate_colliders()
		{
			Relation_object_colider::template clear_related<0>(*this);
		}
		virtual bool is_related_with_engine()override
		{
			return this->is_related();
		}
		virtual Engine_unified& engine()override
		{
			return this->related();
		}
		size_t layer_index()override
		{
			return Object_layer_identifying_by_engine<Object_individual<T>>::layer_index();
		}
		size_t layer_index_in_engine(Engine_unified& engine)override
		{
			return Object_layer_identifying_by_engine<Object_individual<T>>::layer_index_in_engine(engine);
		}
		Engine<T>& layer_engine()
		{
			return this->related();
		}
		void clear_engine_relation()
		{
			if (is_related_with_engine())
				layer_engine().clear_object_relation(*this);
		}
		~Object_individual()
		{
			clear_engine_relation();
			unrelate_colliders();
		}
	};

}