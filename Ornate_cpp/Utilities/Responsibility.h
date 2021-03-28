#pragma once

#include "Certain_responsibility.h"
#include "Programmer_responsibility.h"
#include "Input_responsibility.h"
#include "No_responsibility.h"

namespace uti
{
	template<class Responsibility,class Result>
	using responsibility_result = typename Responsibility::template result_type<Result>;
}