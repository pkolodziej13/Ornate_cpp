#pragma once
#include "Collisions_requirements.h"
namespace col
{
	struct Object_layer_identyfying
	{
		virtual size_t layer_index() = 0;
		virtual size_t layer_index_in_engine(Engine_unified &) = 0;
	};
	template<class Individualist>
	struct Object_layer_identifying_by_engine
	{
		size_t layer_index()
		{
			return static_cast<Individualist&>(*this).layer_engine().get_layer_index();
		}
		size_t layer_index_in_engine(Engine_unified & engine)
		{
			using Layer_engine = Engine<typename Individualist::Layer>;
			Layer_engine& layer_engine = *dynamic_cast<Layer_engine*>(&engine);
			return layer_engine.get_layer_index();
		}
	};

	template<class Layer>
	struct Engine_layers_loader
	{
		virtual size_t layer_index() =0;
	};
	struct Engine_unified_access_to_comparasion
	{
		virtual bool are_layers_of_related_objects_same(Object_unified& a, Object_unified& b) = 0;
		virtual bool layers_of_related_objects_colides(Object_unified& a, Object_unified& b) = 0;
	};
	template<class Comparer, class Implemented_Object_unified>
	struct Engine_unified_access_to_comparasion_implementation
	{
		bool are_layers_of_related_objects_same(Implemented_Object_unified& a, Implemented_Object_unified& b)
		{
			return Comparer::layer_of_indexes_collides(a.layer_index(), b.layer_index());
		}
		bool layers_of_related_objects_colides(Implemented_Object_unified& a, Implemented_Object_unified& b)
		{
			return a.layer_index() == b.layer_index();
		}
	};

	template<class Individualist>
	struct Collider_layers_checking
	{
		template<class Object_type, class Engine_type>
		bool can_absorb_layer_in_engine(Object_type& object, Engine_type& engine)
		{
			bool result = true;
			for_every_object_conected_with_engine(engine, [&](auto& object_c)
			{
				if (engine.are_layers_of_related_objects_same(object, object_c) || engine.layers_of_related_objects_colides(object, object_c))
					result = false;
			});
			return result;
		}
		agg::vector_set<Engine_unified*> engines()
		{
			agg::vector_set<Engine_unified*> result;
			individualist().for_every_object([&](auto& object_c)
			{
				if (object_c.is_related_with_engine())
					result.add(&object_c.engine());
			});
			return result;
		}
		template<class Lambda>
		void for_every_object_conected_with_engine(Engine_unified& engine, Lambda&& lambda)
		{
			individualist().for_every_object([&](auto& object_c)
			{
				if (object_c.is_related_with_engine() && &object_c.engine() == &engine)
					lambda(object_c);
			});
		}

		inline Individualist& individualist()
		{
			return static_cast< Individualist&>(*this);
		}
	};
}