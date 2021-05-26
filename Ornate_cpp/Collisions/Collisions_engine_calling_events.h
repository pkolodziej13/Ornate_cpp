#pragma once
#include "Collisions_requirements.h"
namespace col
{
	namespace Events_requirement
	{

		template<class Individualist,class Comparer>
		struct Events_caller
		{
			template<class Layer1,class Layer2>
			static constexpr size_t contacts_holder()
			{
				using Layers_p = typename Comparer::Layers_p;
				if constexpr (Comparer::template collides<Layer1, Layer2>() && Comparer::template collides<Layer2, Layer1>())
				{
					size_t layer1_index = typ::p_index<Layers_p, Layer1>;
					size_t layer2_index = typ::p_index<Layers_p, Layer2>;
					return (layer1_index <= layer2_index)?0:1;
				}
				else if (Comparer::template collides<Layer1, Layer2>())
					return 0;
				else
					return 1;
			}

			template<class Layer1, class Layer2>
			void collision_enter(Layer1& obj1, Layer2& obj2)
			{
				individualist().add_to_oposite_contacts<Layer1, Layer2>(obj1, obj2);
				if constexpr (std::is_same_v<Layer1, Layer2>)
				{
					if constexpr (Comparer::template collides<Layer1, Layer2>())
						Layer1::on_enter_same_collision(obj1, obj2);
				}
				else
				{
					if constexpr (Comparer::template collides<Layer1, Layer2>())
						obj1.on_enter_collision(obj2);
					if constexpr (Comparer::template collides<Layer2, Layer1>())
						obj2.on_enter_collision(obj1);
				}

			}
			template<class Layer1, class Layer2>
			void during_collision(Layer1& obj1, Layer2& obj2)
			{
				if constexpr (std::is_same_v<Layer1, Layer2>)
				{
					if constexpr (Comparer::template collides<Layer1, Layer2>())
						Layer1::during_same_collision(obj1, obj2);
				}
				else
				{
					if constexpr (Comparer::template collides<Layer1, Layer2>())
						obj1.during_collision(obj2);
					if constexpr (Comparer::template collides<Layer2, Layer1>())
						obj2.during_collision(obj1);
				}
			}
			template<class Layer1, class Layer2>
			void collision_end(Layer1& obj1, Layer2& obj2)
			{
				individualist().remove_from_oposite_contacts<Layer1, Layer2>(obj1, obj2);
				if constexpr (std::is_same_v<Layer1, Layer2>)
				{
					if constexpr (Comparer::template collides<Layer1, Layer2>())
						Layer1::on_exit_same_collision(obj1, obj2);
				}
				else
				{
					if constexpr (Comparer::template collides<Layer1, Layer2>())
						obj1.on_exit_collision(obj2);
					if constexpr (Comparer::template collides<Layer2, Layer1>())
						obj2.on_exit_collision(obj1);
				}
			}
			template<class Layer, class Key>
			Layer& key_to_layer(Key key)
			{
				return individualist().template access_to_objects<Layer>().get_object(key);
			}
			template<class Layer, class Key>
			bool is_valid(Key key)
			{
				return individualist().template access_to_objects<Layer>().is_index_valid(key);
			}


			template<class Evaluating_layer, class Evaluated_layer, class Key, class Contacts>
			void call_events(Key key, Contacts& contacts)
			{
				Evaluating_layer& obj = key_to_layer<Evaluating_layer>(key);
				struct Loop_breaker {};

				auto allowing_to_proceed = [&](auto evaluating_key, auto evaluated_key)->bool
				{
					if (is_valid<Evaluating_layer>(evaluating_key))
					{
						if constexpr (std::is_same_v<Evaluating_layer, Evaluated_layer>)
							return evaluating_key < evaluated_key&& is_valid<Evaluated_layer>(evaluated_key);
						else
							return is_valid<Evaluated_layer>(evaluated_key);
					}
					else
						throw Loop_breaker{};
				};

				auto on_enter = [&](const auto& object_key_access)
				{
					if (allowing_to_proceed(key, object_key_access))
						collision_enter<Evaluating_layer, Evaluated_layer>(obj, key_to_layer<Evaluated_layer>(object_key_access));
				};
				auto on_continue = [&](const auto& object_key_access)
				{
					if (allowing_to_proceed(key, object_key_access))
						during_collision(obj, key_to_layer<Evaluated_layer>(object_key_access));
				};
				auto on_exit = [&](const auto& object_key_access)
				{
					if (allowing_to_proceed(key, object_key_access))
						collision_end(obj, key_to_layer<Evaluated_layer>(object_key_access));
				};

				try
				{
					contacts.new_ones_to_current_remove_safe(on_enter, on_continue, on_exit);
				}
				catch (Loop_breaker)
				{
				}
			}
			template<class Evaluating_layer, class Evaluated_layer>
			void summarize_object_layer_contacts_changes()
			{
				auto lam = [&](auto key_to_object, auto& contacts)
				{
					call_events<Evaluating_layer, Evaluated_layer>(key_to_object, contacts);
				};
				individualist().for_objects_all_contacts<Evaluating_layer, Evaluated_layer>(lam);
			}
			void summarize_layers_contacts_changes()
			{
				using Layers_collisions_p = typename Comparer::Layers_collisions_p;
				Layers_collisions_p::for_each([&](auto L)
				{
					using pair = decl_parameter(L);
					using Layer1 = typ::p_element<0, pair>;
					using Layer2 = typ::p_element<1, pair>;
					if constexpr (contacts_holder<Layer1, Layer2>() == 0)
						summarize_object_layer_contacts_changes<Layer1, Layer2>();
					else
						summarize_object_layer_contacts_changes<Layer2, Layer1>();
				}
				);
			}
			inline Individualist& individualist()
			{
				return static_cast< Individualist&>(*this);
			}
		};
	}
}