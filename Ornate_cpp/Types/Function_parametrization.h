#pragma once
#include <functional>

#include "Pack.h"

namespace typ
{
	template<class Function>
	struct function_prametrization;

	template<class Result, class ... Arguments_v>
	struct function_prametrization<Result(Arguments_v...)>
	{
		using result_type = Result;
		using arguments_types = Pack<Arguments_v...>;
	};
	template<class Result, class ... Arguments_v>
	struct function_prametrization< std::function<Result(Arguments_v...)>>
	{
		using result_type = Result;
		using arguments_types = Pack<Arguments_v...>;
	};
}