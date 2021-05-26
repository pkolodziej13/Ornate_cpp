#pragma once
#include <cmath>
#include <Types/Utility.h>

#include "Utility.h"

namespace uti
{
	template<size_t balast>
	struct Constant_complexity
	{
		constexpr size_t total() 
		{ 
			return balast; 
		}
		template<size_t to_multily>
		Constant_complexity<balast*to_multily> multiply()
		{
			return {};
		}
		template<size_t to_add>
		Constant_complexity<balast+to_add> add()
		{
			return {};
		}
		static constexpr size_t balast_value = balast;
	};

	template<size_t balast>
	struct Linear_complexity
	{
		size_t total() 
		{ 
			return elements* balast; 
		}
		template<size_t to_multily>
		Linear_complexity<balast*to_multily> multiply()
		{
			return { elements };
		}
		template<size_t to_add>
		Linear_complexity<balast + to_add> add()
		{
			return { elements };
		}

		static constexpr size_t balast_value = balast;
		size_t elements;
	};
	template<size_t balast>
	struct Logarythmic_complexity
	{
		size_t total() 
		{ 
			return balast * size_t(std::log(elements) / std::log(2)); 
		}
		template<size_t to_multily>
		Logarythmic_complexity<balast*to_multily> multiply()
		{
			return { elements };
		}
		template<size_t to_add>
		Logarythmic_complexity<balast + to_add> add()
		{
			return { elements };
		}

		static constexpr size_t balast_value = balast;
		size_t elements;
	};

}