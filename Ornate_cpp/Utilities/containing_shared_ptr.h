#pragma once
#include <memory>

#include <Types/Utility.h>

#include "Responsibility.h"
namespace uti
{

	template<class T>
	struct containing_shared_ptr
	{
		using ptr_type = std::shared_ptr<T>;
		template<class T_specific, class = std::enable_if_t<std::is_convertible_v<std::shared_ptr<T_specific>, std::shared_ptr<T>>>>
		containing_shared_ptr(std::shared_ptr<T_specific> with_content)
		{
			uti::Input_responsibility::verify([&]() {return with_content!= nullptr; }, "pointer cannot be empty");

			ptr = with_content;
		}
		template<class T_specific, class = std::enable_if_t<std::is_convertible_v<std::shared_ptr<T_specific>, std::shared_ptr<T>>>>
		containing_shared_ptr(containing_shared_ptr<T_specific> with_content)
		{
			ptr = with_content.shared();
		}
		template<class T_specific, class ... Args_v>
		containing_shared_ptr(typ::Overtype<T_specific>, Args_v && ... args_v)
		{
			ptr = std::make_shared<T_specific>(std::forward<Args_v>(args_v) ...);
		}

		template<class T_specific, class = std::enable_if_t<std::is_convertible_v<std::shared_ptr<T_specific>, std::shared_ptr<T>>>>
		auto& operator =(std::shared_ptr<T_specific> with_content)
		{
			uti::Input_responsibility::verify([&]() {return with_content != nullptr; }, "pointer cannot be empty");

			ptr = with_content;
			return *this;
		}

		T& get()const
		{
			return *ptr.get();
		}
		ptr_type shared()const
		{
			return ptr;
		}
		T* operator->()const
		{
			return ptr.get();
		}
		T& operator*()const
		{
			return get();
		}
		bool operator !()const
		{
			return !ptr;
		}

		auto use_count()const
		{
			return ptr.use_count();
		}

		template<class T_specific, class ... Args_v>
		void reset(Args_v && ... args_av)
		{
			ptr = std::make_shared<T_specific>(std::forward<Args_v>(args_av) ...);
		}
	private:
		ptr_type ptr;
	};

}