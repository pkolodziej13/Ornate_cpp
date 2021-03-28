#pragma once
#include <optional>

namespace uti
{
	template<class T>
	struct Natural_optional
	{
		using type = std::optional<T>;
		template<class ... Args_v>
		static type create(Args_v && ... args_v)
		{
			return std::make_optional<T>(std::forward<Args_v>(args_v)...);
		}
		static type empty()
		{
			return type{};
		}
		static T& value(type & t)
		{
			return t.value();
		}
		static bool has_value(const type& t)
		{
			return t.has_value();
		}
	};
	template<class T>
	struct Natural_optional<T&>
	{
		using  type = T * ;
		template<class ... Args_v>
		static type create(T & value)
		{
			return &value;
		}
		static type empty()
		{
			return nullptr;
		}
		static T& value(type& t)
		{
			return *t;
		}
		static bool has_value(const type& t)
		{
			return t!= nullptr;
		}
	};
	template<>
	struct Natural_optional<void>
	{
		using  type = bool;
		template<class ... Args_v>
		static type create()
		{
			return true;
		}
		static type empty()
		{
			return false;
		}
		static void value(type& t)
		{
			return ;
		}
		static bool has_value(const type& t)
		{
			return t;
		}
	};
}