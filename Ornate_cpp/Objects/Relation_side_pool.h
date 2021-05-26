#pragma once
#include <Utilities/loops.h>
#include <Utilities/Computational_complexity.h>
#include <Aggregations/Pool_unordered.h>

#include "Relation_utilities.h"
#include "Mutual_relation_resolver.h"

namespace obj
{
	struct Relation_side_pool
	{
		using key_to_this = size_t;
		using direct_complexity = uti::Linear_complexity<2>;

		template<class Wing>
		struct Relation_individual
		{
			static constexpr size_t oposite_number = Wing::oposite_type_number;
			using Oposite = typename Wing::Oposite_wing::Individual;
			using Concomitant = typename Wing::Concomitant;
		private:
			friend typename Wing::Master;
			friend Mutual_relation_resolver<typename Wing::Master>;

			template<class Lambda>
			inline void for_each(Lambda&& lambda)
			{
				for (const auto& rel : container.active_range())
				{
					lambda(*rel.related, rel);
				}
			}

			Key_with_result< key_to_this, Concomitant&> add_related_on_this_side(Oposite& oposite, const Concomitant& concomitant)
			{
				auto key = container.add({ oposite,concomitant });
				return { key,{uti::Success_mark{},container[key]} };
			}
			Concomitant& oposite_relation(size_t key)
			{
				return container[key];
			}
			uti::Result_as_success<Concomitant&> oposite_relation(Oposite& oposite)
			{
				for_i(container.total_size())
				{
					if ((container.active_or_not_at(i).related == &oposite) & (container.is_active(i)))
					{
						return { uti::Success_mark{},container[i] };
					}
				}
				return {};
			}
			template<class Lambda>
			inline void action_and_remove_on_this_side(Oposite& object, Lambda&& lambda)
			{
				for_i(container.total_size())
				{
					if (container[i].related == &object)
					{
						lambda(container[i]);
						release_relation(i);
						return;
					}
				}
			}
			void release_relation(size_t key)
			{
				container.free(key);
			}
			void release_relations()
			{
				container.clear();
			}

			direct_complexity finding_complexity()
			{
				return { container.activities_size() };
			}
		private:
			agg::Pool_unordered<Relation_keepment<Oposite, Concomitant>> container;
		};

	};
}