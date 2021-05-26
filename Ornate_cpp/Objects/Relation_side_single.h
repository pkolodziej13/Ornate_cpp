#pragma once
#include <cassert>
#include <Utilities/Computational_complexity.h>

#include "Relation_utilities.h"
#include "Mutual_relation_resolver.h"

namespace obj
{
	struct Relation_side_single
	{
		using key_to_this = void;
		using direct_complexity = uti::Constant_complexity<1>;

		template<class Wing>
		struct Relation_individual
		{
			static constexpr size_t oposite_number = Wing::oposite_type_number;
			using Oposite = typename Wing::Oposite_wing::Individual;
			using Concomitant = typename Wing::Concomitant;
			using Keepment = Relation_keepment<Oposite, Concomitant>;

			auto& related()
			{
				using Maser = typename Wing::Master;
				return Maser::template to_individualist<oposite_number>(*container.related);
			}
			bool is_related()
			{
				return container.related != nullptr;
			}

		private:
			friend typename Wing::Master;
			friend Mutual_relation_resolver<typename Wing::Master>;
			template<class Lambda>
			inline void for_each(Lambda&& lambda)
			{
				if (container.connected())
				{
					lambda(*container.related, static_cast<const Concomitant&>(container));
				}
			}

			Key_with_result< key_to_this, Concomitant&> add_related_on_this_side(Oposite& oposite, const Concomitant& concomitant)
			{
				assert(container.related == nullptr);
				container = Keepment(oposite, concomitant);
				return { uti::Result_as_success < Concomitant&>::create(container) };
			}
			uti::Result_as_success<Concomitant&> oposite_relation(Oposite& oposite)
			{
				if (container.related == &oposite)
					return uti::Result_as_success<Concomitant&>::create(container);
				return {};
			}
			template<class Lambda>
			inline void action_and_remove_on_this_side(Oposite& object, Lambda&& lambda)
			{
				assert(container.related == &object);
				lambda(container);
				container.related = nullptr;
			}
			void release_relations()
			{
				container.related = nullptr;
			}

			uti::Constant_complexity<1> finding_complexity()
			{
				return {};
			}
		private:
			Keepment container;
		};

	};
}