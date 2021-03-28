#pragma once
#include "Pack_utility.h"

namespace typ
{
	template<class function>
	struct Function_deduction;
	template<class Result, class... Arguments>
	struct Function_deduction<Result(Arguments...)>
	{
		using Result_type = Result;
		using Arguments_pack = Pack<Arguments...>;
		using Arguments_synthesized = p_synthesis<Arguments_pack>;
		using Object_type = void;
	};
	template<class Result, class... Arguments>
	struct Function_deduction<Result(*)(Arguments...)>:Function_deduction<Result(Arguments...)>
	{
	};
	template<class Result,class Object, class... Arguments>
	struct Function_deduction<Result(Object::*)(Arguments...)>
	{
		using Result_type = Result;
		using Arguments_pack = Pack<Arguments...>;
		using Arguments_synthesized = p_synthesis<Arguments_pack>;
		using Object_type = Object;
	};


}