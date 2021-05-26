#pragma once
#include <memory>

#include <Types/Utility.h>

#include "Responsibility.h"

namespace uti
{
	template<class T>
	struct holder_ptr
	{
		using this_type = holder_ptr<T>;
		holder_ptr()
		{
			direct = 0;
		}
		template<class T_specific, class = std::enable_if_t<typ::is_base_or_same_v<T, T_specific>>>
		holder_ptr(T_specific* ptr)
		{
			direct = ptr;
		}
		template<class T_specific, class = std::enable_if_t<typ::is_base_or_same_v<T, T_specific>>>
		holder_ptr(std::shared_ptr<T_specific> ptr)
			:owner(ptr), direct(ptr.get())
		{
		}
		template<class T_specific, class = std::enable_if_t<typ::is_base_or_same_v<T, T_specific>>>
		holder_ptr(holder_ptr<T_specific> ptr)
			: owner(ptr.owner), direct(ptr.direct)
		{
		}
		template<class T_specific, class = std::enable_if_t<typ::is_base_or_same_v<T, T_specific>>>
		this_type& operator =(T_specific* ptr)
		{
			owner.reset();
			direct = ptr;
			return *this;
		}
		template<class T_specific, class = std::enable_if_t<typ::is_base_or_same_v<T, T_specific>>>
		this_type& operator = (std::shared_ptr<T_specific> ptr)
		{
			owner = ptr;
			direct = ptr.get();
			return *this;
		}
		template<class T_specific, class = std::enable_if_t<typ::is_base_or_same_v<T, T_specific>>>
		this_type& operator = (holder_ptr<T_specific> ptr)
		{
			owner = ptr.owner;
			direct = ptr.direct;
			return *this;
		}

		operator bool()const
		{
			return direct;
		}
		bool operator !()const
		{
			return direct == 0;
		}
		T* operator ->()
		{
			uti::Input_responsibility::verify([&]() {return direct != nullptr; }, "pointer cannot be empty");
			return direct;
		}
		T& operator *()
		{
			uti::Input_responsibility::verify([&]() {return direct!= nullptr; }, "pointer cannot be empty");
			return *direct;
		}

		inline T* get()const
		{
			return direct;
		}
		void reset()
		{
			owner.reset();
			direct = nullptr;
		}
	private:
		std::shared_ptr<T> owner;
		T* direct;
	};

}