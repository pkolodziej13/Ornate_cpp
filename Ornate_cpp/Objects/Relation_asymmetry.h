#pragma once
#include "Relation_with_value.h"

namespace obj
{
	template<class Relation1_type, class Relation2_type, class T1, class T2, class Value = void, Value_possesing possesing = Value_possesing::optimal>
	struct Relation_asymmetry
	{
		static constexpr size_t oposite_general(size_t I) { return (I + 1) % 2; };
		template<size_t I> using Relation = std::conditional_t<I == 0, Relation1_type, Relation2_type>;
		using Possesing_calculator = Value_possesing_calculator< typename Relation<0>::direct_complexity, typename Relation<1>::direct_complexity, Value, possesing>;
		using value_result = std::add_lvalue_reference_t< Value>;
		using Value_relation = Relation_with_value< Possesing_calculator::possesing_concrete, Value>;
		using Value_part_deduction = Deduce_value_component<Possesing_calculator::possesing_concrete, Value>;

		template<class Master_type,size_t I>
		struct Wing
		{
			using type = std::conditional_t<I == 0, T1, T2>;
			using Concomitant = Relation_concomitant<typename Relation<oposite_general(I)>::key_to_this, typename Value_part_deduction::template Value_component<I>>;
			using Individual = typename Relation<I>::template Relation_individual<Wing<Master_type,I>>;
			static constexpr size_t type_number = I;
			static constexpr size_t has_key = !std::is_same_v<typename Relation<I>::key_to_this, void>;
			static constexpr size_t oposite_type_number = oposite_general(I);
			using Oposite_wing = Wing<Master_type, oposite_general(I)>;
			using Master = Master_type;
		};

		template<class Master_type>
		using Wings_p_temp = typ::Pack<Wing<Master_type,0>,Wing<Master_type,1>>;

		template<class Master_relation,class Relation_head>
		struct Relation_core:
			Value_relation::template Relation_core<Master_relation,
			Relation_core<Master_relation, Relation_head>,
			Wings_p_temp<Master_relation>>
		{
			using this_type = Relation_core<Master_relation, Relation_head>;
			using Wings_p = Wings_p_temp<Master_relation>;
			using base_type = typename Value_relation::template Relation_core<Master_relation,this_type, Wings_p>;
			using Wing1 = typ::p_element<0, Wings_p>;
			using Wing2 = typ::p_element<1, Wings_p>;

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

			friend typename Relation1_type::template Relation_individual<Wing1>;
			friend typename Relation2_type::template Relation_individual<Wing2>;
			friend Master_relation;
			friend Relation_head;
			friend Mutual_relation_resolver<Master_relation>;
		};
	};


}
