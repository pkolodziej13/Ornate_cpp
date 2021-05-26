#pragma once
#include <variant>

#include <Utilities/Index.h>
#include <Types/Utility.h>

namespace obj
{
	template<class Base,class ... Inherited_v>
	std::variant<Inherited_v * ..., std::monostate> discover_type(Base & base)
	{
		std::variant<Inherited_v * ..., std::monostate> result;
		auto check_type = [&](auto over_type)
		{
			using type = typename decltype(over_type)::type;
			type * casted = dynamic_cast<type*>(&base);
			if (casted)
			{
				result.emplace<type*>(casted);
			}
		};
		((check_type(typ::Overtype<Inherited_v>{})), ...);
		return result;
	}
	template<class Inherited_p,class Base>
	uti::Index<size_t> deduce_type(Base& base)
	{
		uti::Index<size_t> result;
		Inherited_p::for_each([&](auto parameter)
		{
			using Type = decl_parameter(parameter);
			if (dynamic_cast<Type*>(&base))
				result = typ::p_index< Inherited_p, Type>;
		});
		return result;
	}
}