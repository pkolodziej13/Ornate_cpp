#pragma once
#include <Utilities/Computational_complexity.h>
#include <Aggregations/Dense.h>

#include "Relation_utilities.h"
#include "Mutual_relation_resolver.h"

namespace obj
{
	struct Relation_side_dense_list
	{
		using key_to_this = void;
		using direct_complexity = uti::Linear_complexity<1>;

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
				for (const auto& rel : container)
				{
					lambda(*rel.related, rel);
				}
			}

			Key_with_result< key_to_this, Concomitant&>  add_related_on_this_side(Oposite& oposite, const Concomitant& concomitant)
			{
				auto key = container.add({ oposite,concomitant });
				return { uti::Result_as_success < Concomitant&>::create(container[key]) };
			}
			uti::Result_as_success<Concomitant&> oposite_relation(Oposite& oposite)
			{
				for_i(container.size())
				{
					if (container[i].related == &oposite)
					{
						return uti::Result_as_success<Concomitant&>::create(container[i]);
					}
				}
				return {};
			}
			template<class Lambda>
			inline void action_and_remove_on_this_side(Oposite& object, Lambda&& lambda)
			{
				for_i(container.size())
				{
					if (container[i].related == &object)
					{
						lambda(container[i]);
						container.swap_remove_at(i);
						return;
					}
				}
			}
			void release_relations()
			{
				container.clear();
			}

			direct_complexity finding_complexity()
			{
				return { container.size() };
			}

		private:
			agg::Dense<Relation_keepment<Oposite, Concomitant>> container;
		};

	};
}