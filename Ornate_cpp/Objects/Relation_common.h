#pragma once
#include <Types/Pack_utility.h>

#include "Mutual_relation_resolver.h"

namespace obj
{
	template<class Master_relation,class Head_relation,class Wings_p>
	struct Relation_common
	{
		using Base_module = Relation_common<Master_relation, Head_relation, Wings_p>;

		template<size_t I> using Individualist = typename typ::p_element<I, Wings_p>::type;
		static constexpr size_t types_size = Wings_p::size;
		template<size_t I> using Relation_wing = typename typ::p_element<I, Wings_p>::Individual;
		template<size_t I> using Concomitant = typename typ::p_element<I, Wings_p>::Concomitant;
		template<size_t I>
		using Wing = typ::p_element<I, Wings_p>;

		static constexpr size_t oposite(size_t I)
		{
			if constexpr (Wings_p::size == 2)
				return (I + 1) % 2;
			else
				return 0;
		};
		using Wing1 = Wing< 0>;
		using Wing2 = Wing<oposite(0)>;
		using Related1 = typename Wing1::type;
		using Related2 = typename Wing2::type;

		static bool relation_exist(Related1& related1, Related2& related2)
		{
			return Mutual_relation_resolver<Master_relation>::template use_any_identification_side<Wing1, Wing2>
				(related1, related2, [](const auto& oposite_relation)->bool
			{
				return oposite_relation.success();
			});
		}

		template<size_t I, class Lambda>
		static void for_each_related(Individualist< I>& object, Lambda&& lambda)
		{
			auto f = [&](Relation_wing<Wing<I>::oposite_type_number>& related, const Concomitant<I>& concomitant)
			{
				lambda(to_individualist<Wing<I>::oposite_type_number>(related));
			};
			Mutual_relation_resolver<Master_relation>::template for_each_identification <Wing<I>>(to_wing<I>(object), f);
		}

		template<size_t I, class Concomitant>
		static auto oposite_concomitant(
			Relation_wing<I>& object1,
			Relation_wing<Wing<I>::oposite_type_number>& object2,
			const Concomitant& key)
		{
			return Mutual_relation_resolver<Master_relation>::template oposite_concomitant<
				Wing<I>, Wing<Wing<I>::oposite_type_number>>(object1, object2, key);
		}
		template<size_t I>
		static auto oposite_concomitant(
			Relation_wing<I>& object1,
			Relation_wing<Wing<I>::oposite_type_number>& object2)
		{
			return Mutual_relation_resolver<Master_relation>::template oposite_concomitant<Wing<I>, Wing<Wing<I>::oposite_type_number>>(object1, object2);
		}

		struct Wing_conversion
		{
		private:
			friend Master_relation;
			friend Head_relation;
			friend Relation_common<Master_relation, Head_relation, Wings_p>;
			template<size_t I>
			inline static Individualist<I>& to_individualist(Relation_wing<I>& base_type)
			{
				return static_cast<Individualist<I>&>(base_type);
			}
			template<size_t I>
			inline static Relation_wing<I>& to_wing( Individualist<I>& base_type)
			{
				return static_cast< Relation_wing<I>&>(base_type);
			}
		};

		template<size_t I>
		inline static Individualist<I>& to_individualist(Relation_wing<I>& base_type)
		{
			return Wing_conversion::to_individualist<I>(base_type);
		}
		template<size_t I>
		inline static Relation_wing<I>& to_wing(Individualist<I>& base_type)
		{
			return Wing_conversion::to_wing<I>(base_type);
		}
	};


}