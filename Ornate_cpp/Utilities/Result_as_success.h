#pragma once
#include "Natural_optional.h"
namespace uti
{
	struct Success_mark {};
	template<class Result_type>
	struct Result_as_success
	{
		using Natural_type = Natural_optional<Result_type>;

		template<class ... Args>
		static Result_as_success<Result_type> create(Args && ... args)
		{
			return Result_as_success<Result_type>(Success_mark{}, std::forward<Args>(args)...);
		}
		Result_as_success()
			: result_hermetized{ Natural_type::empty() }
		{}
		template<class ... Args>
		Result_as_success(Success_mark, Args && ... args)
			: result_hermetized{ Natural_type::create(std::forward<Args>(args)...) }
		{}
		bool success()const
		{
			return Natural_type::has_value(result_hermetized);
		}

		std::add_lvalue_reference_t<Result_type> result()
		{
			return Natural_type::value(result_hermetized);
		}
	private:

		typename Natural_type::type result_hermetized;
	};
}