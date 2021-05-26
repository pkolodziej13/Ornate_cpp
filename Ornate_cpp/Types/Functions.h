#pragma once
#include "Pack_utility.h"

namespace typ
{
	template<class Function>
	struct Function_deduction;
	template<class Result, class... Arguments_v>
	struct Function_deduction<Result(Arguments_v...)>
	{
		using Result_type = Result;
		using Arguments_pack = Pack<Arguments_v...>;
		using Arguments_synthesized = p_synthesis<Arguments_pack>;
		using Object_type = void;
	};
	template<class Result, class... Arguments_v>
	struct Function_deduction<Result(*)(Arguments_v...)>:Function_deduction<Result(Arguments_v...)>
	{
	};
	template<class Result,class Object, class... Arguments_v>
	struct Function_deduction<Result(Object::*)(Arguments_v...)>
	{
		using Result_type = Result;
		using Arguments_p = Pack<Arguments_v...>;
		using Arguments_synthesized = p_synthesis<Arguments_p>;
		using Object_type = Object;
	};


}