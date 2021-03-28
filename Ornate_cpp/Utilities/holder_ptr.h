#pragma once
#include <memory>

#include <Types/Utility.h>

#include "Responsibility.h"
namespace uti
{
	template<class t>
	struct holder_ptr
	{
		using this_type = holder_ptr<t>;
		holder_ptr()
		{
			direct = 0;
		}
		template<class t2, class = std::enable_if_t<typ::is_base_or_same_v<t, t2>>>
		holder_ptr(t2* ptr)
		{
			direct = ptr;
		}
		template<class t2, class = std::enable_if_t<typ::is_base_or_same_v<t, t2>>>
		holder_ptr(std::shared_ptr<t2> ptr)
			:owner(ptr), direct(ptr.get())
		{
		}
		template<class t2, class = std::enable_if_t<typ::is_base_or_same_v<t, t2>>>
		holder_ptr(holder_ptr<t2> ptr)
			: owner(ptr.owner), direct(ptr.direct)
		{
		}
		template<class t2, class = std::enable_if_t<typ::is_base_or_same_v<t, t2>>>
		this_type& operator =(t2* ptr)
		{
			owner.reset();
			direct = ptr;
			return *this;
		}
		template<class t2, class = std::enable_if_t<typ::is_base_or_same_v<t, t2>>>
		this_type& operator = (std::shared_ptr<t2> ptr)
		{
			owner = ptr;
			direct = ptr.get();
			return *this;
		}
		template<class t2, class = std::enable_if_t<typ::is_base_or_same_v<t, t2>>>
		this_type& operator = (holder_ptr<t2> ptr)
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
		t* operator ->()
		{
			uti::Input_responsibility::verify([&]() {return direct != nullptr; }, "pointer cannot be empty");
			return direct;
		}
		t& operator *()
		{
			uti::Input_responsibility::verify([&]() {return direct!= nullptr; }, "pointer cannot be empty");
			return *direct;
		}

		inline t* get()const
		{
			return direct;
		}
		void reset()
		{
			owner.reset();
			direct.reset();
		}
	private:
		std::shared_ptr<t> owner;
		t* direct;
	};

}