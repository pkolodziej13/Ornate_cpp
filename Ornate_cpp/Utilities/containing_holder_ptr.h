#pragma once

#include "holder_ptr.h"

namespace uti
{
	template<class T>
	struct containing_holder_ptr
	{
		using ptr_type = uti::holder_ptr<T>;
		using this_type = containing_holder_ptr<T>;
		template<class T_specific, class ... Args_v, class = std::enable_if_t<std::is_constructible_v<T_specific, Args_v&&...>>>
		containing_holder_ptr(typ::Overtype<T_specific>, Args_v && ... args_v)
		{
			ptr = std::make_shared<T_specific>(std::forward<Args_v>(args_v) ...);
		}
		template< class ... Args_v, class = std::enable_if_t<std::is_constructible_v<T, Args_v&&...>>>
		containing_holder_ptr(Args_v && ... args_v)
		{
			ptr = std::make_shared<T>(std::forward<Args_v>(args_v) ...);
		}
		template<class T_specific, class = std::enable_if_t<std::is_convertible_v<T_specific, ptr_type>>>
		containing_holder_ptr(T_specific ptr_to_set)
			:ptr(ptr_to_set)
		{
			uti::Input_responsibility::verify([&]() {return ptr_to_set != nullptr; }, "pointer cannot be empty");
		}
		template<class T_specific, class = std::enable_if_t<std::is_convertible_v< T_specific, ptr_type>>>
		containing_holder_ptr(containing_holder_ptr<T_specific> ptr_to_set)
			: ptr(ptr_to_set.ptr)
		{
		}
		template<class T_specific, class = std::enable_if_t<std::is_convertible_v<ptr_type, T_specific>>>
		this_type& operator =(T_specific ptr_to_set)
		{
			uti::Input_responsibility::verify([&]() {return ptr_to_set != nullptr; }, "pointer cannot be empty");
			ptr = ptr_to_set;
			return *this;
		}
		template<class T_specific, class = std::enable_if_t<std::is_convertible_v<ptr_type, T_specific>>>
		this_type& operator =(containing_holder_ptr<T_specific> ptr_to_set)
		{
			ptr = ptr_to_set.ptr;
			return *this;
		}

		T& get()const
		{
			return *ptr.get();
		}
		T* operator->()const
		{
			return ptr.get();
		}
		T& operator*()const
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
		template<class T_specific, class ... Args_v>
		void reset(Args_v && ... args_av)
		{
			ptr = std::make_shared<T_specific>(std::forward<Args_v>(args_av) ...);
		}
	private:
		ptr_type ptr;
	};
}