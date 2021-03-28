#pragma once
#include <cassert>
#include <Types/Utility.h>

namespace obj
{
	struct individual_cast
	{
		template<class Individualist, class Individual >
		static inline Individualist * individualist(Individual* individual)
		{
			static_assert(std::is_base_of_v<Individual, Individualist>, "individual must be base of individualist and only of individualist");
			if constexpr (std::is_polymorphic_v<Individual>)
			{
				assert(dynamic_cast<Individualist *>(individual) != nullptr);
			}
			return static_cast<Individualist*>(individual);
		}
		template<class Individualist, class Individual >
		static inline Individual * individual(Individualist* individualist)
		{
			static_assert(std::is_base_of_v<Individual, Individualist>, "individual must be base of individualist and only of individualist");
			return static_cast<Individual*>(individualist);
		}

		template<class Individualist,class Individual_kind >
		static inline auto individual_kind(Individualist* individualist)
		{
			constexpr bool is_kind = typ::is_kind<Individualist, Individual_kind>;
			static_assert(is_kind, "individual must be base of individualist and only of individualist");
			if constexpr (is_kind)
			{
				using concrete_type = decltype(Individual_kind::kind_trial(*individualist));
				return static_cast<concrete_type*>(individualist);
			}
		}
	};


	template<class Individualist, class Individual>
	inline Individualist * individualist(Individual* individual)
	{
		return individual_cast::individualist<Individualist, Individual>(individual);
	}
	template<class Individualist, class Individual>
	inline Individualist & individualist(Individual& individual)
	{
		return *individualist<Individualist>(&individual);
	}

	template<class Individual, class Individualist>
	inline Individual * individual(Individualist* individualist)
	{
		return individual_cast::individual<Individualist, Individual>(individualist);
	}
	template<class Individual, class Individualist>
	inline Individual & individual(Individualist& individualist)
	{
		return *individual<Individual>(&individualist);
	}
	template<class Individual_kind, class Individualist>
	inline auto & individual_kind(Individualist& individualist)
	{
		return *individual_cast::individual_kind< Individualist, Individual_kind >(&individualist);
	}

}