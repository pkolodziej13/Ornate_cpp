#pragma once
#include "Relation_individual_base.h"
#include "Relation_mutual.h"
#include "Relation_symetric.h"
namespace obj
{
	template<class Core_relation>
	struct Relation_final : Core_relation::template Relation_core<Relation_final<Core_relation>, Relation_final<Core_relation>>
	{
		using this_type = Relation_final<Core_relation>;
		using base_type = typename Core_relation::template Relation_core<Relation_final<Core_relation>, Relation_final<Core_relation>>;

		template<size_t I> using Individual = typename Related_base_undeductible< this_type>::template Related_base<I>;


		using base_type::create_relation;
		using base_type::remove_relation;
		using base_type::relation_exist;
		using base_type::relation_value;
		using base_type::get_value_if_exist;
		using base_type::clear_related;
		using base_type::for_each_related;
		using base_type::for_each_related_values;
	private:
		using base_type::to_individualist;
		using base_type::to_wing;
		friend Individual<0>;
		friend Individual<1>;
		friend Mutual_relation_resolver<this_type>;
	};

	template<class Relation1_type, class Relation2_type, class T1, class T2, class Value = void, Value_possesing possesing = Value_possesing::optimal>
	using Relation_mutual = Relation_final<Relation_two_side<Relation1_type, Relation2_type ,T1 ,T2 ,Value ,possesing>>;
	template<class Relation1_type, class Relation2_type, class T1, class T2, class Value = void, Value_possesing possesing = Value_possesing::optimal>
	using Relation_symetric = Relation_final<Relation_symetric_wings<Relation1_type, Relation2_type, T1, T2, Value, possesing>>;
}