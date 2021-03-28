#pragma once

#include <Objects/Relation_utilities.h>
#include <Objects/Relation_individual_base.h>
#include <Objects/Mutual_relation_resolver.h>

#include "Collisions_requirements.h"

namespace col
{
	template<class Engine_type, class Object_type, class Value>
	struct Engine_object_relation;

	template<class T,class Oposite>
	struct Object_side_engine_relation
	{
		using key_to_this = void;
		using direct_complexity = uti::constant_complexity<1>;

		struct Object_individual
		{
			using Concomitant = obj::Relation_concomitant<size_t,void>;
			using Keepment = obj::Relation_keepment<Oposite, Concomitant>;

			Oposite& related()
			{
				return *container.related;
			}
			bool is_related()
			{
				return container.related != nullptr;
			}
			~Object_individual()
			{
				assert(container.related == nullptr);
			}

		private:
			friend Engine_object_relation;
			template<class F>
			inline void for_each(F&& lam)
			{
				if (container.connected())
				{
					lam(*container.related, static_cast<const Concomitant&>(container));
				}
			}

			obj::Key_with_result< key_to_this, Concomitant> add_related_on_this_side(Oposite& oposite, const Concomitant& concomitant)
			{
				assert(container.related == nullptr);
				container = Keepment(oposite, concomitant);
				return { container };
			}
			uti::Result_as_success<Concomitant&> oposite_relation(Oposite& oposite)
			{
				if (container.related == &oposite)
					return { container };
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

			uti::constant_complexity<1> finding_complexity()
			{
				return {};
			}
		private:
			Keepment container;
		};


		template<class Mutual, size_t type_number>
		using relation_individual = Object_individual;

	};
}