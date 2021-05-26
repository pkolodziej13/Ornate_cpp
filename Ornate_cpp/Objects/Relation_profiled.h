#pragma once
#include <Utilities/Utility.h>
#include "Relation_final.h"

namespace obj
{
	template<class Relation, size_t type_number>
	struct Related_profiled_base:Relation::template Individual<type_number>
	{
		static constexpr size_t oposite_number = Relation::oposite(type_number);
		using Oposite_individualist = typename Relation::template Individualist<oposite_number>;

		static constexpr bool non_reverted = type_number ==0;

		template<class ... Constructor_arguments >
		inline void create_relation(Oposite_individualist & oposite, Constructor_arguments && ... constructor_arguments)
		{
			if constexpr (non_reverted)
			{
				Relation::create_relation(this->individualist(), oposite, std::forward<Constructor_arguments>(constructor_arguments)...);
			}
			else
			{
				Relation::create_relation(oposite, this->individualist(), std::forward<Constructor_arguments>(constructor_arguments)...);
			}
		}
		inline void remove_relation(Oposite_individualist & oposite)
		{
			if constexpr (non_reverted)
			{
				Relation::remove_relation(this->individualist(), oposite);
			}
			else
			{
				Relation::remove_relation(oposite, this->individualist());
			}
		}
		inline bool relation_exist(Oposite_individualist & oposite)
		{
			if constexpr (non_reverted)
			{
				return Relation::relation_exist(this->individualist(), oposite);
			}
			else
			{
				return Relation::relation_exist(oposite, this->individualist());
			}
		}
		inline typename Relation::value_result relation_value(Oposite_individualist & oposite)
		{
			if constexpr (Relation::value_is_used)
			{
				if constexpr (non_reverted)
				{
					return Relation::relation_value(this->individualist(), oposite);
				}
				else
				{
					return Relation::relation_value(oposite, this->individualist());
				}
			}
		}
	};

	template<class Relation,class T1, class T2, size_t types_number>
	struct Individuals_selector :Relation
	{
		template<class T>
		static constexpr size_t number_of_type = std::is_same_v<T, T1> ? 0 : 1;

		template<class T>
		using Individual_of = Related_profiled_base< Relation, number_of_type<T>>;

	};

	template<class Relation,class T>
	struct Individuals_selector<Relation,T,T, 2>:Relation
	{
		template<class T>
		struct Individual_of: Relation::template Individual<0>, Relation::template Individual<1>
		{

		};
	};
	template<class Relation, class T>
	struct Individuals_selector<Relation, T, T, 1>:Relation
	{
		template<class T>
		using Individual_of = Related_profiled_base< Relation, 0>;
	};

	template<class Relation>
	struct Relation_profiled
		:Individuals_selector<Relation,typename Relation::template Individualist<0>
		, typename Relation::template Individualist<1>, Relation::types_size >
	{
		using Individual1 = typename Relation::template Individual<0>;
		using Individual2 = typename Relation::template Individual<1>;

		auto get_or_create(typename Relation::template Individualist<0>& related1, 
						   typename Relation::template Individualist<1>& related2)
		{
			auto finding_result = this->get_value_if_exist(related1, related2);
			if (finding_result.success())
				return finding_result.value();
			return this->create_relation(related1, related2);
		}
	};
}