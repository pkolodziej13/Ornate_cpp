#pragma once

#include <Utilities/Comparing_complexity.h>
#include <Utilities/Result_as_success.h>

namespace obj
{
	template<class Relation>
	struct Mutual_relation_resolver
	{
		friend Relation;
		friend typename Relation::Base_module;
		friend typename Relation::Module2;
	private:
		template<class Wing1,class Wing2>
		static auto create_identyfication(typename Wing1::type & related1,typename Wing2::type & related2)
		{
			typename Wing1::Individual & individual1 = Relation::to_wing<Wing1::type_number>(related1);
			typename Wing2::Individual & individual2 = Relation::to_wing<Wing2::type_number>(related2);

			auto result_1 = individual1.add_related_on_this_side(individual2, {});
			auto result_2 = individual2.add_related_on_this_side(individual1, {});
			if constexpr (Wing1::has_key)
			{
				result_2.value.result().key = result_1.key;
			}
			if constexpr (Wing2::has_key)
			{
				result_1.value.result().key = result_2.key;
			}
			std::pair two_successes{ result_1.value, result_2.value };
			return two_successes;
		}
		template<class T1, class T2,class Lambda>
		inline static auto optimal_use_searcher_with_searched(T1& object1, T2& object2, Lambda&& lambda)
		{
			auto finding_complexity_comparsion = uti::compare_complexity(object1.finding_complexity(), object2.finding_complexity());

			if constexpr (finding_complexity_comparsion.true_in_constexpr)
			{
				return lambda(object1, object2);
			}
			else if constexpr (finding_complexity_comparsion.false_in_constexpr)
			{
				return lambda(object2, object1);
			}
			else if (finding_complexity_comparsion.value)
			{
				return lambda(object1, object2);
			}
			else
			{
				return lambda(object2, object1);
			}
		}
		template<class Wing1, class Wing2,class Concomitant>
		static void relese_access(typename Wing1::Individual& individual1, 
								  typename Wing2::Individual& individual2, 
								  const Concomitant & key)
		{
			if constexpr (Wing1::has_key)
			{
				individual1.release_relation(key.key);
			}
			else
			{
				individual1.action_and_remove_on_this_side(individual2, [](auto&&) {});
			}
		}
		template<class Wing1, class Wing2,class Lambda>
		static bool remove_identyfication(typename Wing1::type& related1, typename Wing2::type& related2,Lambda && before_concomitant_remove)
		{
			typename Wing1::Individual& individual1 = Relation::to_wing<Wing1::type_number>(related1);
			typename Wing2::Individual& individual2 = Relation::to_wing<Wing2::type_number>(related2);

			bool result = false;
			optimal_use_searcher_with_searched(individual1, individual2, [&](auto& individual1, auto& individual2)
			{
				constexpr bool not_revereted = std::is_same_v<std::decay_t<decltype(individual1)>, typename Wing1::Individual>;
				using W1 = std::conditional_t<not_revereted, Wing1, Wing2>;
				using W2 = std::conditional_t<!not_revereted, Wing1, Wing2>;
				individual1.action_and_remove_on_this_side(individual2, [&](const auto& concomitant)
				{
					before_concomitant_remove(concomitant);
					relese_access<W2, W1>(individual2,individual1, concomitant);
					result = true;
				});
			});
			return result;
		}

		template<class Wing1, class Wing2,class Lambda>
		static auto use_any_identification_side(typename Wing1::type& related1, typename Wing2::type& related2, Lambda&& lambda)
		{
			typename Wing1::Individual& individual1 = Relation::to_wing<Wing1::type_number>(related1);
			typename Wing2::Individual& individual2 = Relation::to_wing<Wing2::type_number>(related2);

			return optimal_use_searcher_with_searched(individual1, individual2, [&](auto& individual1, auto& individual2)
			{
				return lambda(individual1.oposite_relation(individual2));
			});
		}

		template<class Wing1, class Wing2>
		static auto & get_element_of_identification(typename Wing1::type& related1, typename Wing2::type& related2)
		{
			typename Wing1::Individual& individual1 = Relation::to_wing<Wing1::type_number>(related1);
			typename Wing2::Individual& individual2 = Relation::to_wing<Wing2::type_number>(related2);

			if constexpr (Wing1::has_key)
			{
				auto finding_complexity_comparsion = uti::compare_complexity(individual1.finding_complexity(), individual2.finding_complexity());

				if constexpr (finding_complexity_comparsion.true_in_constexpr)
				{
					return individual1.oposite_relation(individual2).result();
				}
				else if constexpr (finding_complexity_comparsion.false_in_constexpr)
				{
					return individual1.oposite_relation(individual2.oposite_relation(individual1).result().key);
				}
				else if (finding_complexity_comparsion.value)
				{
					return individual1.oposite_relation(individual2).result();
				}
				else
				{
					return individual1.oposite_relation(individual2.oposite_relation(individual1).result().key);
				}
			}
			else
			{
				return individual1.oposite_relation(individual2).result();
			}
		}
		
		template<class Wing1, class Wing2>
		static auto find_element_of_identification(typename Wing1::type& related1, typename Wing2::type& related2)
		{
			typename Wing1::Individual& object_with_result = Relation::to_wing<Wing1::type_number>(related1);
			typename Wing2::Individual& object_without_result = Relation::to_wing<Wing2::type_number>(related2);

			auto& individual1 = Relation::to_individual<Wing1::type_number>(object_with_result);
			auto& individual2 = Relation::to_individual<Wing2::type_number>(object_without_result);

			if constexpr (Wing1::type_number)
			{
				auto finding_complexity_comparsion = uti::compare_complexity(object_with_result.finding_complexity(), object_without_result.finding_complexity().add<1>());

				if constexpr (finding_complexity_comparsion.true_in_constexpr)
				{
					return object_with_result.oposite_relation(individual2).value();
				}
				else if constexpr (finding_complexity_comparsion.false_in_constexpr)
				{
					auto key_finding = object_without_result.oposite_relation(individual1);
					if (key_finding.success())
						return object_with_result.oposite_relation(key_finding.value().key);
					return {};
				}
				else if (finding_complexity_comparsion.value)
				{
					return object_with_result.oposite_relation(individual2).value();
				}
				else
				{
					auto key_finding = object_without_result.oposite_relation(individual1);
					if (key_finding.success())
						return object_with_result.oposite_relation(key_finding.value().key);
					return {};
				}
			}
			else
			{
				auto finding_complexity_comparsion = uti::compare_complexity(object_with_result.finding_complexity(), object_without_result.finding_complexity().multiply<2>().add<1>());

				if constexpr (finding_complexity_comparsion.true_in_constexpr)
				{
					return object_with_result.oposite_relation(individual2);
				}
				else if constexpr (finding_complexity_comparsion.false_in_constexpr)
				{
					auto existance_confirmation = object_without_result.oposite_relation(individual1);
					if (existance_confirmation.success())
						return object_with_result.oposite_relation(individual2);
					return {};
				}
				else if (finding_complexity_comparsion.value)
				{
					return object_with_result.oposite_relation(individual2);
				}
				else
				{
					auto existance_confirmation = object_without_result.oposite_relation(individual1);
					if (existance_confirmation.success())
						return object_with_result.oposite_relation(individual2);
					return {};
				}
			}

		}

		template<class Wing1, class Wing2, class Lambda>
		static void clear_wing(typename Wing1::type& related1, Lambda&& before_concomitant_remove)
		{
			typename Wing1::Individual& wing_to_clear = Relation::to_wing<Wing1::type_number>(related1);

			auto relesing = [&](auto& related, const auto& concomitant)
			{
				before_concomitant_remove(concomitant);
				relese_access<Wing2, Wing1>(related, wing_to_clear, concomitant);
			};
			wing_to_clear.for_each(relesing);
			wing_to_clear.release_relations();
		}

		template<class Wing1, class Wing2, class Concomitant>
		static uti::Result_as_success<typename Wing1::Concomitant&> oposite_concomitant(
			typename Wing1::Individual& individual1 , 
			typename Wing2::Individual& individual2, 
			const Concomitant& key)
		{
			using Result_success = uti::Result_as_success<typename Wing1::Concomitant&>;
			if constexpr (Wing1::has_key)
			{
				return Result_success::create(individual1.oposite_relation(key.key));
			}
			return Result_success::create(individual1.oposite_relation(individual2).result());
		}
		template<class Wing1, class Wing2>
		static uti::Result_as_success<typename Wing1::Concomitant&> oposite_concomitant(typename Wing1::Individual& individual1,typename Wing2::Individual& individual2)
		{
			return individual1.oposite_relation(individual2).result();
		}
		template<class Wing1, class Lambda>
		static void for_each_identification(typename Wing1::Individual& individual1, Lambda&& lambda)
		{
			return individual1.for_each(lambda);
		}

	};
}
