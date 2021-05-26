#pragma once
#include "Mutual_relation_resolver.h"
#include "Relation_individual_base.h"
namespace obj
{
	template<class Core_relation>
	struct Relation_final : Core_relation::template Relation_core<Relation_final<Core_relation>, Relation_final<Core_relation>>
	{
		using this_type = Relation_final<Core_relation>;
		using base_type = typename Core_relation::template Relation_core<Relation_final<Core_relation>, Relation_final<Core_relation>>;

		template<size_t I> using Individual = Relation_individual_base<this_type, I>;


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

}