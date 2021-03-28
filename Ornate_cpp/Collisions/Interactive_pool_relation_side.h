#pragma once
#include <stdexcept>

#include <Utilities/loops.h>

#include <Aggregations/Pool_synchronized.h>
#include <Aggregations/Dense.h>

#include <Objects/Relation_utilities.h>
#include <Objects/Relation_individual_base.h>
#include <Objects/Mutual_relation_resolver.h>

namespace col
{
struct Interactive_pool_relation_side
{
	using key_to_this = size_t;
	using direct_complexity = uti::linear_complexity<2>;

	template<class Mutual, size_t type_number>
	struct relation_individual
	{
		static constexpr size_t oposite_number = Mutual::oposite(type_number);
		using Oposite = typename Mutual::template Relation_wing<oposite_number>;
		using Concomitant = typename Mutual::template Concomitant<type_number>;
		using Keepment = obj::Relation_keepment<Oposite, Concomitant>;

		void refresh()
		{
			container.refresh();
		}
		auto& oposite_of_key(size_t key)
		{
			return Mutual::template to_individualist<oposite_number>(*container.active_or_not_at(key).related);
		}
		bool is_object_under_key_valid(size_t key)
		{
			return container.activity(key) == agg::Live_status::living;
		}

		agg::Pool_synchronized<Keepment> container;
	};

};
}