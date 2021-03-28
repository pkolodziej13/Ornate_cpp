#pragma once
#include "Utility.h"

namespace uti
{
	struct No_invasive_responsibility
	{
		static constexpr bool manual_return = false;
		template<class T>
		using result_type = T;
		template<class T, class ... Args_v>
		static T prepare_result(Args_v && ... args_v)
		{
			return T(std::forward<Args_v>(args_v)...);
		}
		template<class T>
		static T fail_result()
		{
			return T();
		}
		template<class T>
		static T& value_of_result(T& val)
		{
			return val;
		}
	};

	struct Certain :No_invasive_responsibility
	{
		template<class Verify_state>
		static bool verify(Verify_state&& condition_checker)
		{
			return true;
		}
		template<class Verify_state, class Message>
		static bool verify(Verify_state&& condition_checker, const Message& message)
		{
			return true;
		}
	};
}