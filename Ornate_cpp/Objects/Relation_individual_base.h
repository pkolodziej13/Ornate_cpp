#pragma once
namespace obj
{

	template<class Relation_mutual>
	struct Related_base_undeductible
	{
		template<size_t type_number>
		struct Related_base :Relation_mutual::template Relation_wing<type_number>
		{
			using Individualist = typename Relation_mutual::template Individualist<type_number>;

			template<class F>
			inline void for_relations(F && func)
			{
				Relation_mutual::template for_each_related<type_number>(individualist(), func);
			}
			template<class F>
			inline void for_relations_values(F && func)
			{
				Relation_mutual::template for_each_related_values<type_number>(individualist(), func);
			}

			inline void clear_relations()
			{
				Relation_mutual::clear_related<type_number>(individualist());
			}

			inline ~Related_base()
			{
				clear_relations();
			}
		protected:
			inline Individualist& individualist()
			{
				return Relation_mutual::to_individualist<type_number>(*this);
			}
		};
	};

}