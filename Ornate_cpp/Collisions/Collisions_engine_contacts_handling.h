#pragma once
#include <Types/Functions_parametrized_pointers.h>
#include "Collisions_engine_object_completed.h"
#include "Collisions_engine_calling_events.h"

namespace col
{
	template<class Individualist,class Comparer>
	struct Contacts_handling:
		Events_requirement::Events_caller<Individualist, Comparer>
	{
		using this_type = Contacts_handling<Individualist, Comparer>;

		template<class Layer>
		using Relation_for_layer = Relation_engine_object<Layer, Individualist, Comparer >;

		template<class Layer1, class Layer2, class Lambda>
		void for_objects_all_contacts(Lambda&& lambda)
		{
			using Relation = Relation_for_layer<Layer1>;
			auto lam = [&](auto& obj, auto& value)
			{
				lambda(Relation::key_of_object(obj), value.get<Layer2>());
			};

			Relation::for_each_related_values<0>(individualist(), lam);
		}
		struct Calling_set_contact
		{
			template<size_t colision_number>
			static void call(this_type & context,Object_unified& object1, Object_unified& object2)
			{
				using First_type = typ::p_element < 0, typ::p_element< colision_number, typename Comparer::Layers_collisions_p>>;
				using Second_type = typ::p_element < 1, typ::p_element< colision_number, typename Comparer::Layers_collisions_p>>;
				Object_individual<First_type>* first = dynamic_cast<Object_individual<First_type>*>(&object1);
				Object_individual<Second_type>* second = dynamic_cast<Object_individual<Second_type>*>(&object2);

				using Relation1 = Relation_for_layer<First_type>;
				using Relation2 = Relation_for_layer<Second_type>;

				auto set_contacts = [&]()
				{
					if constexpr (Comparer::template collides<First_type, Second_type>())
					{
						auto& contacts_of_first1 = Relation1::relation_value(context.individualist(), *first).get< Second_type>();
						contacts_of_first1.collect(Relation2::key_of_object(*second));
					}
					if constexpr (Comparer::template collides<Second_type, First_type>())
					{
						auto& contacts_of_first2 = Relation2::relation_value(context.individualist(), *second).get< First_type>();
						contacts_of_first2.collect(Relation1::key_of_object(*first));
					}
				};

				if constexpr (std::is_same_v<First_type, Second_type>)
				{
					if (&object1 != &object2)
						set_contacts();
				}
				else
					set_contacts();
			}
		};

		template<class Connections, size_t I1, size_t I2>
		void set_contacs(const Connections& connections, std::array<Object_unified*, I1>const & objects1, std::array<Object_unified*, I2>const& objects2)
		{
			using callers_table = typ::Functions_parametrized_pointers<Calling_set_contact, Comparer::collisons_number>;
			for (const auto& object_contacts : connections)
			{
				auto& object1 = *objects1[object_contacts.first_rejestr_element];
				auto& object2 = *objects2[object_contacts.second_rejestr_element];
				if (object_contacts.not_reversed)
					(*callers_table::value[object_contacts.collision_number])(*this,object1, object2);
				else
					(*callers_table::value[object_contacts.collision_number])(*this, object2, object1);
			}
		}


		template<class Layer1, class Layer2>
		void remove_form_contacts(Layer1& object)
		{

			using Relation1 = Relation_for_layer <Layer1>;

			auto& contacts = Relation1::relation_value(individualist(), object).get< Layer2>();
			contacts.clear_new();
			this->call_events<Layer1, Layer2>(Relation1::key_of_object(object), contacts);
		}

		template<class Layer>
		void remove_contacts_of(Object_individual<Layer>& object)
		{
			using all_collidng = typename Comparer::template Colliding_objects<Layer>;
			all_collidng::for_each([&](auto parameter)
			{
				using colliding = decl_parameter(parameter);
				remove_form_contacts<Layer, colliding>(static_cast<Layer&>(object));
			});
		}

		template<class Layer1, class Layer2>
		void add_to_oposite_contacts(Layer1& obj1, Layer2& obj2)
		{
			if constexpr (Comparer::template collides<Layer2, Layer1>())
			{
				using Relation1 = Relation_for_layer<Layer1 >;
				using Relation2 = Relation_for_layer<Layer2 >;
				auto& contacts = Relation2::relation_value(individualist(), obj2).get< Layer1>();
				contacts.add_current(Relation1::key_of_object(obj1));
			}
		}
		template<class Layer1, class Layer2>
		void remove_from_oposite_contacts(Layer1& obj1, Layer2& obj2)
		{
			if constexpr (Comparer::template collides<Layer2, Layer1>())
			{
				using Relation1 = Relation_for_layer<Layer1 >;
				using Relation2 = Relation_for_layer<Layer2 >;
				auto& contacts = Relation2::relation_value(individualist(), obj2).get< Layer1>();
				contacts.erase(Relation1::key_of_object(obj1));
			}
		}
		inline Individualist& individualist()
		{
			return static_cast<Individualist&>(*this);
		}
	};
}