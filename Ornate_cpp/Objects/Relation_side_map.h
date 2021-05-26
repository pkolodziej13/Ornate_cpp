#pragma once
#include <set>
#include <map>
#include <Utilities/Computational_complexity.h>

#include "Relation_utilities.h"
#include "Mutual_relation_resolver.h"

namespace obj
{
	struct Relation_side_map
	{
		using key_to_this = void;
		using direct_complexity = uti::Logarythmic_complexity<2>;

		template<class Wing>
		struct Relation_individual;

		template< class Wing, class Concomitant>
		struct relation_specjalized
		{
			static constexpr size_t oposite_number = Wing::oposite_type_number;
			using Oposite = typename Wing::Oposite_wing::Individual;
		private:
			friend typename Wing::Master;
			friend Mutual_relation_resolver<typename Wing::Master>;
			template<class Lambda>
			inline void for_each(Lambda&& lambda)
			{
				for (const auto& rel : container)
				{
					lambda(*rel.first, rel.second);
				}
			}

			Key_with_result< key_to_this, Concomitant&> add_related_on_this_side(Oposite& oposite, const Concomitant& concomitant)
			{
				auto& content = container[&oposite];
				content = concomitant;
				return { uti::Result_as_success < Concomitant&>::create(content) };
			}
			uti::Result_as_success<Concomitant&> oposite_relation(Oposite& object)
			{
				auto iterator = container.find(&object);
				if (iterator != container.end())
					return uti::Result_as_success<Concomitant&>::create((*iterator).second);
				return {};
			}
			template<class Lambda>
			inline void action_and_remove_on_this_side(Oposite& object, Lambda&& lambda)
			{
				auto iterator = container.find(&object);
				lambda((*iterator).second);
				container.erase(iterator);
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
			std::map<Oposite*, Concomitant> container;
		};
		template<class Wing>
		struct relation_specjalized<Wing, Relation_concomitant<void, void> >
		{
			using Concomitant = Relation_concomitant<void, void>;
			static constexpr size_t oposite_number = Wing::oposite_type_number;
			using Oposite = typename Wing::Oposite_wing::Individual;
		private:
			friend typename Wing::Master;
			friend Mutual_relation_resolver<typename Wing::Master>;
			template<class Lambda>
			inline void for_each(Lambda&& lambda)
			{
				for (const auto& rel : container)
				{
					lambda(*rel, Concomitant{});
				}
			}

			Key_with_result< key_to_this, void> add_related_on_this_side(Oposite& oposite, const Concomitant&)
			{
				container.emplace(&oposite);
				return { uti::Success_mark{} };
			}
			uti::Result_as_success <void> oposite_relation(Oposite& oposite)
			{
				if (container.find(&oposite) != container.end())
					return { uti::Success_mark{} };
				return {};
			}
			template<class Lambda>
			inline void action_and_remove_on_this_side(Oposite& oposite, Lambda&& lambda)
			{
				lambda(Concomitant{});
				container.erase(&oposite);
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
			std::set<Oposite*> container;
		};

		template<class Wing>
		struct Relation_individual :relation_specjalized<Wing, typename Wing::Concomitant>
		{
		};
	};
}