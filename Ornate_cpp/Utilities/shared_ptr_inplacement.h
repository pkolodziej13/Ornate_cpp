#pragma once
#include <memory>

namespace uti
{
	template<class T>
	struct shared_ptr_inplacement
	{
		shared_ptr_inplacement() {}
		shared_ptr_inplacement(T const& object)
		{
			element = std::make_shared<T>(object);
		}
		shared_ptr_inplacement(std::shared_ptr<T> object)
		{
			element = object;
		}
		template<class T_specific, class = std::enable_if_t<std::is_convertible_v<T_specific, T> || std::is_convertible_v<T_specific, std::shared_ptr<T>> >>
		shared_ptr_inplacement(const T_specific& object)
		{
			using convertion_type = std::conditional_t<std::is_convertible_v<T_specific, T>, T, std::shared_ptr<T>>;
			convertion_type converted = object;
			auto a = shared_ptr_inplacement(converted);
			element = a.element;
		}
		std::shared_ptr<T> get()const
		{
			return element;
		}
	private:
		std::shared_ptr<T> element;
	};
}