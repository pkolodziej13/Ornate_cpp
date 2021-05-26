#pragma once
#include <Utilities/Non_replicable.h>
#include <Utilities/Responsibility.h>

namespace obj
{
	template<class Identificator_type, class Responsibility>
	struct Instance_global_single :uti::non_replicable
	{
		Instance_global_single()
		{
			Responsibility::verify([&]() {return current == nullptr; });
			current = this;
		}
		~Instance_global_single()
		{
			Responsibility::verify([&]() {return current == this; });
			current = nullptr;
		}

		static bool can_be_created()
		{
			return current == nullptr;
		}
	private:
		inline static Instance_global_single < Identificator_type, Responsibility >* current{ nullptr };
	};
	template<class Identificator_type>
	struct Instance_global_single<Identificator_type, uti::Certain> :uti::non_replicable
	{
		static bool can_be_created()
		{
			return true;
		}
	};
}