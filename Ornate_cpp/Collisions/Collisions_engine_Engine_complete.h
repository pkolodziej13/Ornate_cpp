#pragma once
#include "Collisions_engine_relations.h"

#include "Collisions_engine_collider_rejestr.h"
#include "Collisions_engine_redirecting_Engine_collider_relation_events.h"
#include "Collisions_engine_matching_colliders_with_objects.h"
#include "Collisions_engine_layers_collisions.h"
#include "Collisions_engine_rejestring_colliders_considering_objects_number.h"

#include "Collisions_engine_contacts_handling.h"

namespace col
{
	struct Engine_unified
		:virtual Engine_unified_relation_side,
		Engine_unified_access_to_comparasion
	{};

	template<class T>
	struct Engine :
		virtual Engine_unified
	{
		friend Engine_object_relation;
		friend Engine_braid_fiber;
		friend Events_requirement::Events_caller;

		virtual void relate_with_object(T & object) = 0;
		virtual void unrelate_object(T& object) = 0;
		virtual void unrelate_objects() = 0;
		virtual size_t get_layer_index() = 0;
		virtual void clear_object_relation(Object_individual<T> &) = 0;

	protected:
		virtual T& get_object(size_t key) = 0;
		virtual bool is_index_valid(size_t key) = 0;
	};

	template<class Object, class Objects_p, class Coliders_p>
	struct Engine_braid_fiber<Object, Engine_braid<Objects_p, Coliders_p>>
		:Engine<Object>,
		Relation_engine_object<Object, Engine_braid<Objects_p, Coliders_p>, typ::p_transform<Engine_layers_comparer, Objects_p> >::template Individual<0>
	{
		using Individualist = Engine_braid<Objects_p, Coliders_p>;
		using relation_type = typename  Relation_engine_object<Object, Engine_braid<Objects_p, Coliders_p>, typ::p_transform<Engine_layers_comparer, Objects_p> >;
		virtual size_t get_layer_index()override
		{
			return typ::p_transform<Engine_layers_comparer, Objects_p>::template layer_key<Object>();
		}

		virtual void relate_with_object(Object& object)override
		{
			relation_type::create_relation(individualist(), object);
		}
		virtual void unrelate_object(Object& object)override
		{
			relation_type::remove_relation(individualist(), object);
		}
		virtual void unrelate_objects()override
		{
			relation_type::clear_related<0>(individualist());
		}
		virtual Object& get_object(size_t key)override
		{
			return static_cast<Object&>(relation_type::object_of_key(individualist(), key));
		}
		virtual bool is_index_valid(size_t key)override
		{
			return relation_type::is_index_valid(individualist(), key);
		}
		virtual void clear_object_relation(Object_individual<Object>& object)override
		{
			relation_type::clear_related<1>(object);
		}
	private:
		Individualist& individualist()
		{
			return static_cast<Individualist&>(*this);
		}
	};
	
	template<class ... Objects_v, class Coliders_p>
	struct Engine_braid<typ::Pack<Objects_v...>, Coliders_p>
		:uti::variadic_inheritance< Engine_braid_fiber<Objects_v, Engine_braid<typ::Pack<Objects_v...>, Coliders_p>>...>,
		Engine_relation_side<Engine_braid<typ::Pack<Objects_v...>, Coliders_p>>,
		Rejestring_colliders_considering_objects_number<Engine_braid<typ::Pack<Objects_v...>, Coliders_p>,
			Engine_layers_comparer< Objects_v...>, Colliders_rejestr<Coliders_p>::template Rejestr>,
		Contacts_handling<Engine_braid<typ::Pack<Objects_v...>, Coliders_p>,Engine_layers_comparer< Objects_v...>>,
		Engine_unified_access_to_comparasion_implementation< Engine_layers_comparer< Objects_v...>, Object_unified>
	{
		using Comparer = Engine_layers_comparer< Objects_v...>;
		using this_type = Engine_braid<typ::Pack<Objects_v...>, Coliders_p>;
		using Layers_p = typ::Pack<Objects_v...>;

		friend Engine_object_relation;
		template<class Layer>
		Engine<Layer>& access_to_objects()
		{
			return *this;
		}
		void detect_contacts()
		{
			this->evaluate_colliders([&](const auto & connections_of_keys,auto& collider1, auto& collider2)
			{
				if (collider1.is_colliding(collider2))
				{
					this->set_contacs(connections_of_keys, collider1.get_objects(), collider2.get_objects());
				}
			});
		}
		void synchronize_object_relations()
		{
			auto synchronize = [&](auto parameter)
			{
				using type = decl_parameter(parameter);
				using Relation = Relation_engine_object<type, this_type, Comparer >;
				using Individual = typename Relation::template Individual<0>;
				Individual& related_wing = *this;
				related_wing.refresh();
			};
			typ::Pack<Objects_v...>::for_each(synchronize);
		}

		void update_collisions_state()
		{
			synchronize_object_relations();
			this->synchronize();
			this->detect_contacts();
			this->summarize_layers_contacts_changes();
		}
		bool are_layers_of_related_objects_same(Object_unified& a, Object_unified& b)override
		{
			return Engine_unified_access_to_comparasion_implementation< Engine_layers_comparer< Objects_v...>, Object_unified>::
				are_layers_of_related_objects_same(a, b);
		}
		bool layers_of_related_objects_colides(Object_unified& a, Object_unified& b)override
		{
			return Engine_unified_access_to_comparasion_implementation< Engine_layers_comparer< Objects_v...>, Object_unified>::
				layers_of_related_objects_colides(a, b);
		}

		template<class T>
		void relate_with_object(T & object)
		{
			access_to_objects<T>().relate_with_object(object);
		}

		~Engine_braid()
		{
			Layers_p::for_each([&](auto parameter)
			{
				using type = decl_parameter(parameter);
				using relation = Relation_engine_object<type, this_type, Engine_layers_comparer< Objects_v...> >;
				relation::clear_related<0>(*this);
			});
		}
	};
}