#pragma once

#include <Objects/Relation_utilities.h>
#include <Objects/Relation_individual_base.h>
#include <Objects/Mutual_relation_resolver.h>

#include <Collisions/Interactive_pool_relation_side.h>

#include "Collisions_requirements.h"
#include "Object_side_engine_relation.h"

namespace col
{
	template<class Engine_type, class Layer, class Value>
	struct Engine_object_relation
	{
		template<class Master_relation, class Relation_head>
		struct Relation_core
		{
			static constexpr size_t types_size = 2;
			static constexpr size_t oposite(size_t I) { return (I + 1) % 2; };
			template<size_t I> using Individualist = std::conditional_t < I == 0, Engine_type, Object_individual<Layer>> ;
			template<size_t I> using Concomitant = std::conditional_t < I == 0, obj::Relation_concomitant<void, Value>, obj::Relation_concomitant<size_t, void>>;

			using Object_type = Object_individual<Layer>;

			using Engine_wing = typename col::Interactive_pool_relation_side::template Relation_individual<Relation_core<Master_relation, Relation_head>,0>;
			using Object_wing = typename Object_side_engine_relation<Object_individual<Layer>, Engine<Layer>>::Object_individual ;
			template<size_t I> using Relation_wing = std::conditional_t < I == 0
				, Engine_wing
				, Object_wing>;

			static uti::Result_as_success<void> create_relation(Engine_type& related1, Object_type& related2)
			{
				Engine_wing& engine_wing = related1;
				Object_wing& object_wing = related2;
				object_wing.container.key = engine_wing.container.add({ related2 });
				object_wing.container.related = &related1;

				return uti::Result_as_success<void>::create();
			}
			static bool remove_relation(Engine_type& related1, Object_type& related2)
			{
				Engine_wing& engine_wing = related1;
				Object_wing& object_wing = related2;
				engine_wing.container.free(object_wing.container.key);
				object_wing.container.related = nullptr;
				return true;
			}
			static bool relation_exist(Engine_type& related1, Object_type& related2)
			{
				Object_wing& object_wing = related2;
				return object_wing.container.related == related1;
			}
			static Value& relation_value(Engine_type& related1, Object_type& related2)
			{
				Engine_wing& engine_wing = related1;
				Object_wing& object_wing = related2;
				return engine_wing.container.active_or_not_at(object_wing.container.key).value;
			}
			static uti::Result_as_success<Value&> get_value_if_exist(Engine_type& related1, Object_type& related2)
			{
				Engine_wing& engine_wing = related1;
				Object_wing& object_wing = related2;
				if (relation_exist(related1, related2))
					return uti::Result_as_success<Value>(engine_wing.container.active_or_not_at(object_wing.container.key));
				return uti::Result_as_success<Value>::failure();
			}
			template<size_t number_of_selected>
			static void clear_related(Individualist<number_of_selected>& base)
			{
				Relation_wing<number_of_selected>& wing = base;
				if constexpr (number_of_selected == 0)
				{
					for_i(wing.container.total_size())
						if (wing.container.activity(i) == agg::Live_status::initiated || wing.container.activity(i) == agg::Live_status::living)
						{
							wing.container.free(i);
							wing.container.active_or_not_at(i).related->container.related = nullptr;
						}
				}
				else
				{
					remove_relation(*dynamic_cast<Engine_type*>(wing.container.related), base);
				}
			}
			template<size_t number_of_selected, class Lambda>
			static void for_each_related(Individualist< number_of_selected>& related, Lambda&& lambda)
			{
				Relation_wing<number_of_selected>& wing = related;
				if constexpr (number_of_selected == 0)
				{
					for_i(wing.container.total_size())
						if ( wing.container.activity(i) == agg::Live_status::living)
						{
							lambda(to_individualist<1>(*wing.container.active_or_not_at(i).related));
						}
				}
				else
				{
					if (wing.container.related)
						lambda(*dynamic_cast<Engine_type*>(wing.container.related));
				}
			}
			template<size_t number_of_selected, class Lambda>
			static void for_each_related_values(Individualist< number_of_selected>& related, Lambda&& lambda)
			{
				Relation_wing<number_of_selected>& wing = related;
				if constexpr (number_of_selected == 0)
				{
					for_i(wing.container.total_size())
						if (wing.container.activity(i) == agg::Live_status::living)
						{
							lambda(to_individualist<1>(*wing.container.active_or_not_at(i).related), wing.container.active_or_not_at(i).value);
						}
				}
				else
				{
					if(wing.container.related)
						lambda(*dynamic_cast<Engine_type*>(wing.container.related), wing.container.related->container.active_or_not_at(wing.container.key).value);
				}
			}

			template<size_t selected>
			static Individualist< selected> & to_individualist(Relation_wing< selected>& related)
			{
				return static_cast<Individualist< selected>&>(related);
			}
			template<size_t selected>
			static Relation_wing< selected>& to_wing(Individualist< selected>& related)
			{
				return static_cast<Relation_wing< selected>&>(related);
			}
			static size_t key_of_object(Object_type& object)
			{
				Object_wing& object_wing = object;
				return object_wing.container.key;
			}
			static Object_type&  object_of_key(Engine_type& engine,size_t key)
			{
				Engine_wing& engine_wing = engine;
				return to_individualist<1>(*engine_wing.container.active_or_not_at(key).related);
			}
			static bool is_index_valid(Engine_type& engine, size_t key)
			{
				Engine_wing& engine_wing = engine;
				return engine_wing.container.activity(key) == agg::Live_status::living;
			}
		};
	};
}