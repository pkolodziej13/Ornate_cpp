#pragma once
#include <stdexcept>

#include "Certain_responsibility.h"

namespace uti
{
	struct Input_responsibility :No_invasive_responsibility
	{
		template<class Verify_state>
		static bool verify(Verify_state&& condition_checker)
		{
			bool result = condition_checker();
			if (!result)
				throw std::runtime_error("input error");
			return result;
		}
		template<class Verify_state, class Message>
		static bool verify(Verify_state&& condition_checker, const Message& message)
		{
			bool result = condition_checker();
			if (!result)
				throw std::runtime_error(message);
			return result;
		}
	};
	
}
