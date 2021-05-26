#pragma once
#include <Utilities/Computational_complexity.h>
#include <Aggregations/Dense.h>

#include "Relation_utilities.h"
#include "Mutual_relation_resolver.h"

namespace obj
{
	struct Relation_side_dense_indexed
	{

		using key_to_this = size_t;
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

			Key_with_result< key_to_this, Concomitant&> add_related_on_this_side(Oposite& oposite, const Concomitant& concomitant)
			{
				auto key = container.add({ oposite,concomitant });
				return { key,uti::Result_as_success < Concomitant&>::create(container[key]) };
			}
			Concomitant& oposite_relation(size_t key)
			{
				return container[key];
			}
			uti::Result_as_success <Concomitant&> oposite_relation(Oposite& object)
			{
				for_i(container.size())
				{
					if (container[i].related == &object)
					{
						return uti::Result_as_success <Concomitant&>::create(container[i]);
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
						release_relation(i);
						return;
					}
				}
			}
			void release_relation(size_t key)
			{
				container.swap_remove_at(key);
				if (container.size() > key)
				{
					using Maser = typename Wing::Master;
					Maser::template oposite_concomitant<Wing::oposite_type_number>(
						*container[key].related,
						*this,
						container[key]).result().key = key;
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
