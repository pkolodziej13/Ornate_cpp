#pragma once
#include <memory>

#include <Types/Utility.h>

#include "Responsibility.h"
namespace uti
{

	template<class t>
	struct containing_shared_ptr
	{
		using ptr_type = std::shared_ptr<t>;
		template<class t2, class = std::enable_if_t<std::is_convertible_v<std::shared_ptr<t2>, std::shared_ptr<t>>>>
		containing_shared_ptr(std::shared_ptr<t2> with_content)
		{
			uti::Input_responsibility::verify([&]() {return with_content!= nullptr; }, "pointer cannot be empty");

			ptr = with_content;
		}
		template<class t2, class = std::enable_if_t<std::is_convertible_v<std::shared_ptr<t2>, std::shared_ptr<t>>>>
		containing_shared_ptr(containing_shared_ptr<t2> with_content)
		{
			ptr = with_content.shared();
		}
		template<class stored_type, class ... args_v>
		containing_shared_ptr(typ::Overtype<stored_type>, args_v && ... args_av)
		{
			ptr = std::make_shared<stored_type>(std::forward<args_v>(args_av) ...);
		}

		template<class t2, class = std::enable_if_t<std::is_convertible_v<std::shared_ptr<t2>, std::shared_ptr<t>>>>
		auto& operator =(std::shared_ptr<t2> with_content)
		{
			uti::Input_responsibility::verify([&]() {return with_content != nullptr; }, "pointer cannot be empty");

			ptr = with_content;
			return *this;
		}

		t& get()const
		{
			return *ptr.get();
		}
		ptr_type shared()const
		{
			return ptr;
		}
		t* operator->()const
		{
			return ptr.get();
		}
		t& operator*()const
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

		template<class stored_type, class ... args_v>
		void reset(args_v && ... args_av)
		{
			ptr = std::make_shared<stored_type>(std::forward<args_v>(args_av) ...);
		}
	private:
		ptr_type ptr;
	};

}