#pragma once
#include <cmath>
#include <Types/Utility.h>

#include "Utility.h"

namespace uti
{
	template<size_t balast>
	struct constant_complexity
	{
		static constexpr size_t balast_value = balast;
		constexpr size_t total() { return balast; }
		template<size_t to_multily>
		constant_complexity<balast*to_multily> multiply()
		{
			return {};
		}
		template<size_t to_add>
		constant_complexity<balast+to_add> add()
		{
			return {};
		}
	};
	template<size_t balast>
	struct linear_complexity
	{
		static constexpr size_t balast_value = balast;
		size_t elements;
		size_t total() { return elements* balast; }
		template<size_t to_multily>
		linear_complexity<balast*to_multily> multiply()
		{
			return { elements };
		}
		template<size_t to_add>
		linear_complexity<balast + to_add> add()
		{
			return { elements };
		}
	};
	template<size_t balast>
	struct logarythmic_complexity
	{
		static constexpr size_t balast_value = balast;
		size_t elements;
		size_t total() { return balast * size_t(std::log(elements) / std::log(2)); }
		template<size_t to_multily>
		logarythmic_complexity<balast*to_multily> multiply()
		{
			return { elements };
		}
		template<size_t to_add>
		logarythmic_complexity<balast + to_add> add()
		{
			return { elements };
		}
	};

}