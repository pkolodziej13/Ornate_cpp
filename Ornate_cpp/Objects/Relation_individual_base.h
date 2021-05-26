#pragma once
namespace obj
{
	template<class Relation_mutual,size_t type_number>
	struct Relation_individual_base :Relation_mutual::template Relation_wing<type_number>
	{
		using Individualist = typename Relation_mutual::template Individualist<type_number>;

		template<class Lambda>
		inline void for_relations(Lambda&& lambda)
		{
			Relation_mutual::template for_each_related<type_number>(individualist(), lambda);
		}
		template<class Lambda>
		inline void for_relations_values(Lambda&& lambda)
		{
			Relation_mutual::template for_each_related_values<type_number>(individualist(), lambda);
		}

		inline void clear_relations()
		{
			Relation_mutual::clear_related<type_number>(individualist());
		}

		inline ~Relation_individual_base()
		{
			clear_relations();
		}
	protected:
		inline Individualist& individualist()
		{
			return Relation_mutual::to_individualist<type_number>(*this);
		}
	};
}