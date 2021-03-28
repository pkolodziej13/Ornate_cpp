#pragma once
#include "Relation_mutual.h"
#include "Relation_individual_base.h"
#include "Relation_with_value.h"

namespace obj
{
	template<class Relation1_type, class Relation2_type, class T1, class T2, class Value = void, Value_possesing possesing = Value_possesing::optimal>
	struct Relation_symetric_wings :Relation_two_side<Relation1_type, Relation2_type, T1, T2, Value, possesing>
	{
	};
	template<class Relation, class T, class Value, Value_possesing possesing>
	struct Relation_symetric_wings<Relation, Relation, T, T, Value, possesing>
	{
		static constexpr Value_possesing_concrete possesing_concrete = std::is_same_v<void, Value>? Value_possesing_concrete::none: Value_possesing_concrete::both;
		using Value_part_deduction = Deduce_value_component<possesing_concrete, Value>;
		using Value_relation = Relation_with_value< possesing_concrete, Value>;

		template<class Master_type, size_t I>
		struct Wing_temp
		{
			using type = T;
			using Concomitant = Relation_concomitant<typename Relation::key_to_this, typename Value_part_deduction::template Value_component<I>>;
			using Individual = typename Relation::template relation_individual<Wing_temp<Master_type, I>>;
			static constexpr size_t type_number = I;
			static constexpr size_t has_key = !std::is_same_v<typename Relation::key_to_this, void>;
			using Oposite_wing = Wing_temp<Master_type, I>;
			using Master = Master_type;
		};

		template<class Master_type>
		using Wings_p = typ::Pack<Wing_temp<Master_type, 0>>;

		template<class Master_relation, class Relation_head>
		struct Relation_core :
			Value_relation::template Relation_core<
				Master_relation,
				Relation_core<Master_relation, Relation_head>,
				Wings_p<Master_relation>>
		{
			using this_type = Relation_core<Master_relation, Relation_head>;
			using Wings_p = Wings_p<Master_relation>;
			using base_type = typename Value_relation::template Relation_core<Master_relation,this_type,Wings_p>;
			using Wing1 = typ::p_element<0, Wings_p>;

		private:
			using base_type::create_relation;
			using base_type::remove_relation;
			using base_type::relation_exist;
			using base_type::relation_value;
			using base_type::get_value_if_exist;
			using base_type::clear_related;
			using base_type::for_each_related_values;
		private:
			using base_type::optimal_acces_to_concomitant;
			using base_type::value_from_access;

			using base_type::oposite_concomitant;
			using base_type::to_individualist;
			using base_type::to_wing;

			friend typename Relation::template relation_individual<Wing1>;
			friend Master_relation;
			friend Relation_head;
			friend Mutual_relation_resolver<Master_relation>;
		};
	};

}