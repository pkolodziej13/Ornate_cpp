#pragma once
#include "Certain_responsibility.h"
#include "Natural_optional.h"

namespace uti
{
	struct No_responsibility
	{
		template<class Verify_state>
		static bool verify(Verify_state&& condition_checker)
		{
			return condition_checker();
		}
		template<class Verify_state, class Message>
		static bool verify(Verify_state&& condition_checker, const Message& message)
		{
			return condition_checker();
		}

		template<class T>
		using result_type = typename Natural_optional<T>::type;
		template<class T, class ... Args_v>
		static result_type<T> prepare_result(Args_v && ... args_v)
		{
			return Natural_optional<T>::create(std::forward<Args_v>(args_v)...);
		}
		template<class T>
		static result_type<T> fail_result()
		{
			return Natural_optional<T>::empty();
		}
		template<class T>
		static T& value_of_result(result_type<T>& val)
		{
			return Natural_optional<T>::value(val);
		}

		static constexpr bool manual_return = true;
	};

}