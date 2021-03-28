#pragma once
#include <functional>

#include "Pack.h"

namespace typ
{
	template<class func>
	struct function_prametrization;

	template<class result_t, class ... arguments_t_v>
	struct function_prametrization<result_t(arguments_t_v...)>
	{
		using result_type = result_t;
		using arguments_types = Pack<arguments_t_v...>;
	};
	template<class result_t, class ... arguments_t_v>
	struct function_prametrization< std::function<result_t(arguments_t_v...)>>
	{
		using result_type = result_t;
		using arguments_types = Pack<arguments_t_v...>;
	};
}