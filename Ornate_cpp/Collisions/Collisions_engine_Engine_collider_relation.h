#pragma once
#include <bitset>

#include <Types/Pack.h>

#include "Synchronized_vector.h"
#include "Collisions_requirements.h"

namespace col
{
	template<class Individualist>
	struct Collider_relation_side
	{
		void add_pool_index_for_engine(Engine_unified & engine,size_t pool_index) 
		{
			engine_pool_indexes.emplace_for(individualist().engines(), &engine, pool_index);
		}
		size_t engine_place(Engine_unified& engine) 
		{
			return engine_pool_indexes.get(individualist().engines(), &engine);
		}
		void change_engine_index_on_this_side(Engine_unified& engine, size_t pool_index) 
		{
			engine_pool_indexes.set_for(individualist().engines(),&engine, pool_index);
		}
		size_t removed_engine_pool_index(Engine_unified& engine)
		{
			return engine_pool_indexes.get(individualist().engines(),&engine);
		}
		void remove_engine_on_this_side(Engine_unified& engine) 
		{
			engine_pool_indexes.remove_element_of(individualist().engines(), &engine);
		}
		Synchronized_vector<size_t> engine_pool_indexes;

		inline Individualist& individualist()
		{
			return static_cast< Individualist&>(*this);
		}
	};
	template<class Individualist, class Comparer>
	struct Engine_collider_relation_wing
	{
		using Layers_p = typename Comparer::Layers_p;
		using Layers_key = std::bitset<Layers_p::size>;

		template<class Collider_type>
		Layers_key get_key_of_collider(Collider_type& collider)
		{
			Layers_key result;
			auto lam = [&](auto& object)
			{
				result.set(object.layer_index());
			};

			collider.for_every_object_conected_with_engine(individualist().individualist(), lam);
			return result;
		}
		template<class Object_type>
		Layers_key remove_layer_from_key( Layers_key  key, Object_type& object)
		{
			key.reset(object.layer_index());
			return key;
		}
		template<class Object_type>
		Layers_key add_layer_to_key( Layers_key key, Object_type& object)
		{
			size_t index = object.layer_index_in_engine(individualist().individualist());
			key.set(index);
			return key;
		}

		template<class Collider_type>
		size_t add_collider_on_this_side( Collider_type& collider, const Layers_key& key)
		{
			return individualist().add_collider_on_this_side(collider, key);
		}
		template<class Collider_type>
		size_t change_collider_on_this_side( Collider_type& collider,size_t old_place, const Layers_key& old_key, const Layers_key& new_key)
		{
			return individualist().change_collider_on_this_side(collider, old_place, old_key, new_key);
		}
		template<class Collider_type>
		void remove_collider_on_this_side(Collider_type& collider, size_t old_place, const Layers_key& old_key)
		{
			return individualist().remove_collider_on_this_side(collider, old_place, old_key);
		}
		inline Individualist& individualist()
		{
			return static_cast<Individualist&>(*this);
		}
	};

	template<class Collecting_engine>
	struct Engine_collider_relation
	{
		using layers_key = typename Collecting_engine::Layers_key;
		template<class Collider_type>
		static void verify_relation_after_add(Collecting_engine& engine, Collider_type& collider, Object_unified& object)
		{
			auto current_layers_key = engine.get_key_of_collider(collider);
			auto previus_layers_key = engine.remove_layer_from_key(current_layers_key, object);

			if (previus_layers_key.none())
			{
				size_t place = engine.add_collider_on_this_side(collider, current_layers_key);
				collider.add_pool_index_for_engine(engine, place);
			}
			else
			{
				size_t old_place = collider.engine_place(engine);
				size_t place = engine.change_collider_on_this_side(collider, old_place, previus_layers_key, current_layers_key);
				collider.change_engine_index_on_this_side(engine, place);
			}
		}
		template<class Collider_type>
		static void verify_relation_after_remove(Collecting_engine& engine, Collider_type& collider, Object_unified& object)
		{
			auto current_layers_key = engine.get_key_of_collider(collider);
			auto previus_layers_key = engine.add_layer_to_key(current_layers_key, object);

			if (current_layers_key.none())
			{
				size_t old_place = collider.removed_engine_pool_index(engine);
				engine.remove_collider_on_this_side(collider, old_place, previus_layers_key);
				collider.remove_engine_on_this_side(engine);
			}
			else
			{
				size_t old_place = collider.engine_place(engine);
				size_t place = engine.change_collider_on_this_side(collider, old_place, previus_layers_key, current_layers_key);
				collider.change_engine_index_on_this_side(engine, place);
			}
		}
	};



}