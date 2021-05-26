#pragma once
#include <deque>
#include <vector>
#include <cassert>

#include <Utilities/Non_replicable.h>


namespace obj
{
	template< class Eventer, class Base_relation>
	struct Relation_with_events
	{
		template<class Master_relation, class Head_relation, size_t type_number>
		struct Related_events_base :Head_relation::template Relation_wing<type_number>, uti::non_replicable
		{
			using Individualist = typename Master_relation::template Individualist<type_number>;
			inline ~Related_events_base()
			{
				size_t i = 0;
				auto cound_related = [&](auto& related)
				{
					i++;
				};
				Master_relation::template for_each_related<type_number>(individualist(), cound_related);

				auto check = [&]()->bool { return i == 0; };

				assert(("call clear_relations in higher destructor", check()));
			}
		private:
			inline Individualist& individualist()
			{
				return Master_relation::template to_individualist<type_number>(*this);
			}
		};


		template<class Master_relation, class Relation_head>
		struct Relation_core:Base_relation::template Relation_core<Master_relation, Relation_core<Master_relation, Relation_head>>
		{
			using base_type = typename Base_relation::template Relation_core<Master_relation, Relation_core<Master_relation, Relation_head>>;
			using this_type = Relation_core<Master_relation, Relation_head>;


			template<size_t I> using Individualist = typename base_type::template Individualist<I>;
			template<size_t I> using Relation_wing = Related_events_base< this_type, base_type,I>;

			static constexpr size_t I2 = base_type::types_size == 2 ? 1 : 0;

		private:
			template<class ... Value_argumets_v>
			inline static auto create_relation(Individualist<0> &related1, Individualist<I2>&related2, Value_argumets_v && ... value_argumets_v)
			{
				typename Eventer::On_create eventer;
				constexpr bool has_condition = std::is_same_v<decltype(eventer.before_create(related1, related2)), bool>;
				if constexpr (has_condition)
				{
					using result_type = decltype(base_type::create_relation(related1, related2, std::forward<Value_argumets_v>(value_argumets_v)...));
					if (!eventer.before_create(related1, related2))
						return result_type{};
				}
				else
				{
					eventer.before_create(related1, related2);
				}
				auto creation_result = base_type::create_relation(related1, related2, std::forward<Value_argumets_v>(value_argumets_v)...);
				if (creation_result.success())
				{
					eventer.after_create(related1, related2);
				}
				return creation_result;
			}
			inline static bool remove_relation(Individualist<0>&related1, Individualist<I2>&related2)
			{
				typename Eventer::On_remove eventer(related1, related2);
				bool result = base_type::remove_relation(related1, related2);
				if(result)
					eventer.after_remove(related1, related2);
				return result;
			}			
			template<size_t number_of_selected>
			static void clear_related(Individualist<number_of_selected>& base)
			{
				std::deque<typename Eventer::On_remove> eventers;
				std::vector<std::add_pointer_t<Individualist<base_type::oposite(number_of_selected)>> > remembered_related;
				auto cound_related = [&](auto& related)
				{
					if constexpr (I2 == number_of_selected)
						eventers.emplace_back(related, base);
					else
						eventers.emplace_back(base,related);
					remembered_related.push_back(&related);
				};
				Master_relation::template for_each_related<number_of_selected>(base, cound_related);

				base_type::template clear_related<number_of_selected>(base);

				for (size_t i = 0; i < eventers.size(); ++i)
				{
					if constexpr (I2 == number_of_selected)
						eventers[i].after_remove(*remembered_related[i], base);
					else
						eventers[i].after_remove(base, *remembered_related[i]);
				}
			}
			using base_type::relation_exist;
			using base_type::relation_value;
			using base_type::get_value_if_exist;
			using base_type::for_each_related;
			using base_type::for_each_related_values;
			using base_type::to_individualist;
			using base_type::to_wing;

			friend Related_events_base;
			friend Master_relation;
			friend Relation_head;
		};
	};
}