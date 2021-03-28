#pragma once

#include "holder_ptr.h"
namespace uti
{
	template<class t>
	struct containing_holder_ptr
	{
		using ptr_type = uti::holder_ptr<t>;
		using this_type = containing_holder_ptr<t>;
		template<class stored_type, class ... args_v, class = std::enable_if_t<std::is_constructible_v<stored_type, args_v&&...>>>
		containing_holder_ptr(typ::Overtype<stored_type>, args_v && ... args_av)
		{
			ptr = std::make_shared<stored_type>(std::forward<args_v>(args_av) ...);
		}
		template< class ... args_v, class = std::enable_if_t<std::is_constructible_v<t, args_v&&...>>>
		containing_holder_ptr(args_v && ... args_av)
		{
			ptr = std::make_shared<t>(std::forward<args_v>(args_av) ...);
		}


		template<class t2, class = std::enable_if_t<std::is_convertible_v<t2, ptr_type>>>
		containing_holder_ptr(t2 ptr_a)
			:ptr(ptr_a)
		{
			uti::Input_responsibility::verify([&]() {return ptr_a != nullptr; }, "pointer cannot be empty");
		}
		template<class t2, class = std::enable_if_t<std::is_convertible_v< t2, ptr_type>>>
		containing_holder_ptr(containing_holder_ptr<t2> ptr_a)
			: ptr(ptr_a.ptr)
		{
		}

		template<class t2, class = std::enable_if_t<std::is_convertible_v<ptr_type, t2>>>
		this_type& operator =(t2 ptr_a)
		{
			uti::Input_responsibility::verify([&]() {return ptr_a != nullptr; }, "pointer cannot be empty");
			ptr = ptr_a;
			return *this;
		}
		template<class t2, class = std::enable_if_t<std::is_convertible_v<ptr_type, t2>>>
		this_type& operator =(containing_holder_ptr<t2> ptr_a)
		{
			ptr = ptr_a.ptr;
			return *this;
		}



		t& get()const
		{
			return *ptr.get();
		}

		t* operator->()const
		{
			return ptr.get();
		}
		t& operator*()const
		{
			return get();
		}
		ptr_type raw()const
		{
			return ptr;
		}
		operator ptr_type()const
		{
			return ptr;
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