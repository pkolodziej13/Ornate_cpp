#pragma once
#include "holder_ptr.h"
#include "containing_holder_ptr.h"
#include "containing_shared_ptr.h"

namespace uti
{
	template<class t>
	struct shared_ptr_inplacement
	{
		shared_ptr_inplacement() {}
		shared_ptr_inplacement(const t& object)
		{
			element.reset(new t(object));
		}
		shared_ptr_inplacement(std::shared_ptr< t> object)
		{
			element = object;
		}
		template<class _t, typename = std::enable_if_t<std::is_convertible_v<_t, t> || std::is_convertible_v<_t, std::shared_ptr<t>> >>
		shared_ptr_inplacement(const _t& object)
		{
			using convertion_type = std::conditional_t<std::is_convertible_v<_t, t>, t, std::shared_ptr<t>>;
			convertion_type converted = object;
			auto a = shared_ptr_inplacement(converted);
			element = a.element;
		}

		std::shared_ptr<t> element;
	};
}