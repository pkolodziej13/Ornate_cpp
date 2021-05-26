#pragma once
#include <vector>

#include <Utilities/Non_replicable.h>
#include <Utilities/Responsibility.h>

namespace obj
{
	template<class Identificator_type, class Responsibility>
	struct Instance_global_indexing :uti::non_replicable
	{
		Instance_global_indexing(size_t unit) :unit(unit)
		{
			if (unit >= current.size())
			{
				current.resize(unit + 1, nullptr);
			}
			Responsibility::verify([&]() {return current[unit] == nullptr; });
			current[unit] = this;
		}
		~Instance_global_indexing()
		{
			Responsibility::verify([&]() {return current[unit] == this; });
			current[unit] = nullptr;
		}

		static bool can_be_created(size_t unit)
		{
			return current[unit] == nullptr;
		}
		size_t get_unit()const
		{
			return unit;
		}
	private:
		inline static std::vector<Instance_global_indexing<Identificator_type, Responsibility>*> current;

		size_t unit;
	};
	template<class Identificator_type>
	struct Instance_global_indexing<Identificator_type, uti::Certain> :uti::non_replicable
	{
		Instance_global_indexing(size_t  unit)
			:unit(unit)
		{}
		static bool can_be_created(size_t  unit)
		{
			return true;
		}
		size_t get_unit()const
		{
			return unit;
		}
	private:
		size_t unit;
	};
}