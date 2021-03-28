#pragma once
#include <cassert>
#include "Debug.h"
#include "Certain_responsibility.h"

namespace uti
{
	struct Programmer_responsibility_debug :No_invasive_responsibility
	{
		template<class Verify_state>
		static bool verify(Verify_state&& condition_checker)
		{
			bool result = condition_checker();
			assert(result);
			return result;
		}
		template<class Verify_state,class Message>
		static bool verify(Verify_state&& condition_checker,const Message & message)
		{
			bool result = condition_checker();
			assert(result);
			return result;
		}
	};
	using Programmer_responsibility = uti::By_configuration<Programmer_responsibility_debug, Certain>;
}