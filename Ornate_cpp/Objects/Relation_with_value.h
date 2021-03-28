#pragma once
#include <cassert>
#include <Types/Pack_indexing.h>
#include "Relation_utilities.h"
#include "Relation_common.h"
#include "Value_possesing.h"

namespace obj
{

	template<Value_possesing_concrete possesing,class Value>
	struct Relation_with_value
	{

		template<size_t I>
		using Value_component =typename  Deduce_value_component<possesing, Value>::template Value_component<I>;
		using value_result = std::add_lvalue_reference_t< Value>;
		static constexpr bool value_is_used = possesing != Value_possesing_concrete::none;
		static constexpr size_t type_keepeing_value = Deduce_value_component<possesing, Value>::type_keeping_value;

		template<class Master_relation, class Relation_head,class Wings_p>
		struct Relation_core :
			Relation_common<Master_relation,Relation_core<Master_relation, Relation_head, Wings_p>,Wings_p>
		{
			using base_type = Relation_common<Master_relation, Relation_core<Master_relation, Relation_head, Wings_p>, Wings_p>;
			using this_type = Relation_core<Master_relation, Relation_head, Wings_p>;
			using Module2 = this_type;

			template<size_t I>
			using Wing = typ::p_element<I, Wings_p>;

			template<size_t I> using Individualist = typename base_type::template Individualist<I>;
			template<size_t I> using Relation_wing = typename base_type::template Relation_wing<I>;
			template<size_t I> using Concomitant = typename Wing<I>::Concomitant;

			using Wing1 = Wing< 0>;
			using Wing2 = Wing<Wing1::oposite_type_number>;
			using Related1 = typename Wing1::type;
			using Related2 = typename Wing2::type;

		private:
			template<class ... Value_argumets_v>
			static uti::Result_as_success<value_result> create_relation(Related1& related1, Related2& related2, Value_argumets_v && ... value_argumets_v)
			{
				using Result_success = uti::Result_as_success<value_result>;
				if constexpr (std::is_same_v<Wing1, Wing2>)
				{
					assert(&related1 != &related2);
				}
				assert(!base_type::relation_exist(related1, related2));
				auto result = Mutual_relation_resolver<Master_relation>::template create_identyfication<Wing1, Wing2>(related1, related2);

				if constexpr (possesing == Value_possesing_concrete::both)
				{
					Value* created_value = new Value(std::forward<Value_argumets_v>(value_argumets_v)...);
					result.first.value().value = created_value;
					result.second.value().value = created_value;
					return Result_success::create( *created_value );
				}
				else if constexpr (possesing == Value_possesing_concrete::first)
				{
					result.first.result().value = Value(std::forward<Value_argumets_v>(value_argumets_v)...);
					return Result_success::create(result.first.result().value );
				}
				else if constexpr (possesing == Value_possesing_concrete::second)
				{
					result.second.result().value = Value(std::forward<Value_argumets_v>(value_argumets_v)...);
					return Result_success::create(result.second.result().value );
				}
				else
				{
					return Result_success::create();
				}
			}
			static bool remove_relation(Related1& related1, Related2& related2)
			{
				assert(base_type::relation_exist(related1, related2));

				return Mutual_relation_resolver<Master_relation>::template remove_identyfication<Wing1, Wing2>
					(related1, related2, [](const auto& concomitant)
				{
					if constexpr (possesing == Value_possesing_concrete::both)
					{
						delete concomitant.value;
					}
				});
			}
			using base_type::relation_exist;
			static value_result relation_value(Related1& related1, Related2& related2)
			{
				if constexpr (possesing == Value_possesing_concrete::both)
				{
					return *Mutual_relation_resolver<Master_relation>::template use_any_identification_side<Wing1, Wing2>
						(related1, related2, [](const auto& oposite_relation)
					{
						return oposite_relation.value().value;
					});
				}
				else if constexpr (value_is_used)
				{
					return optimal_acces_to_concomitant<type_keepeing_value>(related1, related2).value;
				}
			}
			static uti::Result_as_success<value_result> get_value_if_exist(Related1& related1, Related2& related2)
			{
				if constexpr (possesing == Value_possesing_concrete::both)
				{
					return Mutual_relation_resolver<Master_relation>::template use_any_identification_side<Wing1, Wing2>(related1, related2, [](const auto& oposite_relation)->value_result
					{
						return uti::Result_as_success<value_result>::create( oposite_relation.value());
					});
				}
				else if constexpr (value_is_used)
				{
					auto founded_concomitant = optimal_conditional_acces_to_concomitant<type_keepeing_value>(related1, related2);
					if (founded_concomitant.success())
						return founded_concomitant.value();
					return {};
				}
				else
				{
					return uti::Result_as_success<value_result>::create(relation_exist(related1, related2) );
				}
			}
			template<size_t number_of_selected>
			static void clear_related(Individualist<number_of_selected>& base)
			{
				auto remove_beside_constexpr_error = [](const auto& concomitant)
				{
					if constexpr (possesing == Value_possesing_concrete::both)
					{
						delete concomitant.value;
					}
				};
				if constexpr (number_of_selected == 0)
					Mutual_relation_resolver<Master_relation>::template clear_wing<Wing1, Wing2>
					(base, remove_beside_constexpr_error);
				else
					Mutual_relation_resolver<Master_relation>::template clear_wing<Wing2, Wing1>
					(base, remove_beside_constexpr_error);
			}

			using base_type::for_each_related;
			template<size_t I, class Lambda>
			static void for_each_related_values(Individualist< I>& object, Lambda&& lambda)
			{
				static_assert(value_is_used, "relation is not using value");

				auto f = [&](Relation_wing<oposite_general(I)>& related, const Concomitant<I>& concomitant)
				{
					lambda(to_individualist<oposite_general(I)>(related), value_from_access<I>(to_wing<I>(object), related, concomitant));
				};
				Mutual_relation_resolver<Master_relation>::template for_each_identification < Wing< I>>(object, f);
			}
		private:
			template<size_t I>
			static Concomitant<I>& optimal_acces_to_concomitant(Related1& related1, Related2& related2)
			{
				if constexpr (I == 0)
					return Mutual_relation_resolver<Master_relation>::template get_element_of_identification<Wing1, Wing2>
					(related1, related2);
				else
					return Mutual_relation_resolver<Master_relation>::template get_element_of_identification<Wing2, Wing1>
					(related2, related1);

			}
			template<size_t I>
			static uti::Result_as_success <Concomitant<I>&> optimal_conditional_acces_to_concomitant(Related1& related1, Related2& related2)
			{
				if constexpr (I == 0)
					return Mutual_relation_resolver<Master_relation>::template find_element_of_identification<Wing1, Wing2>
					(related1, related2);
				else
					return Mutual_relation_resolver<Master_relation>::template find_element_of_identification<Wing2, Wing1>
					(related2, related1);
			}


			template<size_t I>
			static inline value_result value_from_access(
				Relation_wing<I>& object_with_concomitant,
				Relation_wing<base_type::oposite(I)>& object2, const Concomitant<I>& concomitant)
			{
				if constexpr (possesing == Value_possesing_concrete::both)
				{
					*concomitant.value;
				}
				if constexpr (value_is_used)
				{
					if constexpr (type_keepeing_value == I)
						return oposite_concomitant<base_type::oposite(I)>(object2, object_with_concomitant, concomitant).value;
					else
						return concomitant.value;
				}
			}

			using base_type::oposite_concomitant;
			using base_type::to_individualist;
			using base_type::to_wing;

			friend typename Wing1::template Individual;
			friend typename Wing2::template Individual;
			friend Master_relation;
			friend Relation_head;
			friend Mutual_relation_resolver<Master_relation>;
		};


	};
}